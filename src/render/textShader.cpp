#include "../../include/render/textShader.hpp"

#include "../../include/app.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>

static constexpr const unsigned char vertSrc[] = {
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wc23-extensions"
#endif
#embed "../../res/shader/compiled/textRender.vert.spv"
};

static constexpr const unsigned char fragSrc[] = {
#embed "../../res/shader/compiled/textRender.frag.spv"
#ifdef __clang__
	#pragma clang diagnostic pop
#endif
};

using Bloodlust::TextShader;

TextShader::TextShader(
	SDL_GPUDevice* device,
	SDL_Window* window,
	TTF_TextEngine* engine,
	bool& success
) {
	SDL_GPUSamplerCreateInfo samplerInfo = {};
	samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
	samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
	samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
	samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	sampler = SDL_CreateGPUSampler(device, &samplerInfo);
	if (sampler == nullptr) {
		success = false;
		return;
	}

	font = TTF_OpenFont(FONT_PATH, 72.0f);
	if (font == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open font");
		success = false;
		return;
	}
	TTF_SetFontWrapAlignment(font, TTF_HORIZONTAL_ALIGN_CENTER);

	VertexAttributeDefinition attribDefs[] = {
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3},
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4},
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2}
	};
	success = init(
		device,
		window,
		{.src = vertSrc,
		 .vertexAttribDefs = attribDefs,
		 .srcLen = sizeof(vertSrc),
		 .vertBufferSize = MAX_VERTEX_COUNT * sizeof(Vertex),
		 .indexBufferSize = MAX_INDEX_COUNT * sizeof(int32_t),
		 .vertexAttribCount = 3,
		 .uniformBufferCount = 1},
		{.src = fragSrc, .srcLen = sizeof(fragSrc), .samplerCount = 1}
	);
}

bool TextShader::queueTextSequence(const TextInfo& textInfo, TTF_GPUAtlasDrawSequence* sequence) {
	bool isDataFree = true;
	size_t vertCount = size_t(sequence->num_vertices);
	size_t indexCount = size_t(sequence->num_indices);
	if (vertCount > MAX_VERTEX_COUNT - geometryData.vertexCount) {
		vertCount = MAX_VERTEX_COUNT - geometryData.vertexCount;
		isDataFree = false;
	}
	if (indexCount > MAX_INDEX_COUNT - geometryData.indexCount) {
		indexCount = MAX_INDEX_COUNT - geometryData.indexCount;
		isDataFree = false;
	}

	for (size_t i = 0; i < vertCount; i++) {
		const SDL_FPoint pos = sequence->xy[i];
		Vertex vert = {.pos = {pos.x, pos.y, 0.0f}, .color = textInfo.color, .uv = sequence->uv[i]};
		geometryData.vertices[geometryData.vertexCount + i] = vert;
	}
	SDL_memcpy(
		geometryData.indices + geometryData.indexCount,
		sequence->indices,
		indexCount * sizeof(int32_t)
	);

	geometryData.vertexCount += vertCount;
	geometryData.indexCount += indexCount;
	return isDataFree;
}

void TextShader::setGeometryData() {
	Vertex* transferData = (Vertex*) SDL_MapGPUTransferBuffer(device, vertTransBuf, false);
	SDL_memcpy(transferData, geometryData.vertices, sizeof(Vertex) * geometryData.vertexCount);
	SDL_memcpy(
		transferData + MAX_VERTEX_COUNT,
		geometryData.indices,
		sizeof(int32_t) * geometryData.indexCount
	);
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
}

void TextShader::onResize(uint32_t width, uint32_t height) {
	matrices[0] = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.1f, 100.0f);
}

void TextShader::pushText(const TextInfo& textData) {
	if (textCount == MAX_TEXT_COUNT) {
		return;
	}
	data[textCount] = textData;
	textCount++;
}

void TextShader::clearData() {
	for (uint32_t i = 0; i < textCount; i++) {
		TTF_DestroyText(textObjects[i]);
		textObjects[i] = nullptr;
	}
	textCount = 0;
	geometryData.vertexCount = 0;
	geometryData.indexCount = 0;
}

void TextShader::prepareShader() {
	if (textCount == 0) {
		return;
	}

	glm::mat4 model = {1.0f};
	model = glm::translate(model, {640.0f, 360.0f, 0.0f});
	matrices[1] = model;

	for (uint32_t i = 0; i < textCount; i++) {
		TextInfo textInfo = data[i];
		TTF_Text* textObject = TTF_CreateText(app->renderState.textEngine, font, textInfo.str, 0);
		textObjects[i] = textObject;
		TTF_GPUAtlasDrawSequence* sequence = TTF_GetGPUTextDrawData(textObject);
		for (; sequence != nullptr; sequence = sequence->next) {
			if (!queueTextSequence(textInfo, sequence)) {
				goto exitLoop;
			}
		}
	}

exitLoop:
	// Get the text data and queue the text in a buffer for drawing later
	setGeometryData();
}

void TextShader::runCopyPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPUCopyPass* copyPass) {
	if (textCount == 0) {
		return;
	}
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = sizeof(Vertex) * MAX_VERTEX_COUNT
	};
	SDL_GPUBufferRegion vertRegion = {
		.buffer = vertBuffer,
		.offset = 0,
		.size = uint32_t(sizeof(Vertex) * geometryData.vertexCount)
	};
	SDL_GPUBufferRegion indexRegion = {
		.buffer = indexBuffer,
		.offset = 0,
		.size = uint32_t(sizeof(int32_t) * geometryData.indexCount)
	};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
}

void TextShader::runRenderPass(
	SDL_GPUCommandBuffer* cmdbuf,
	SDL_GPURenderPass* renderPass,
	SDL_GPUTexture* swapTex
) {
	if (textCount == 0) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
	SDL_PushGPUVertexUniformData(cmdbuf, 0, matrices, sizeof(matrices));

	int vertexOffset = 0, indexOffset = 0;
	for (uint32_t i = 0; i < textCount; i++) {
		TTF_Text* textObject = textObjects[i];
		TTF_GPUAtlasDrawSequence* seq = TTF_GetGPUTextDrawData(textObject);
		for (; seq != nullptr; seq = seq->next) {
			SDL_GPUTextureSamplerBinding samplerBinding = {
				.texture = seq->atlas_texture,
				.sampler = sampler
			};

			bool shouldBreak = false;
			int vertCount = seq->num_vertices;
			int indexCount = seq->num_indices;
			if (vertCount > MAX_VERTEX_COUNT - vertexOffset) {
				vertCount = MAX_VERTEX_COUNT - vertexOffset;
				shouldBreak = true;
			}
			if (indexCount > MAX_INDEX_COUNT - indexOffset) {
				indexCount = MAX_INDEX_COUNT - indexOffset;
				shouldBreak = true;
			}

			// note: due to a bug in sdl, this call can trigger undefined behaviour when running on
			// vulkan, which is scheduled to be fixed in 3.2.18
			// (see https://github.com/libsdl-org/sdl/issues/13191)
			SDL_BindGPUFragmentSamplers(renderPass, 0, &samplerBinding, 1);
			SDL_DrawGPUIndexedPrimitives(
				renderPass,
				uint32_t(indexCount),
				1,
				uint32_t(indexOffset),
				vertexOffset,
				0
			);

			vertexOffset += vertCount;
			indexOffset += indexCount;
			if (shouldBreak) {
				return;
			}
		}
	}
}

TextShader::~TextShader() {
	for (TTF_Text* v : textObjects) {
		TTF_DestroyText(v);
	}
	SDL_ReleaseGPUSampler(device, sampler);
	TTF_CloseFont(font);
}
