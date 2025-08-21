#include "../../include/render/textShader.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/font.hpp"
#include "../../include/render/shaderSrc/textRender.frag.hpp"
#include "../../include/render/shaderSrc/textRender.vert.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::TextShader;

TextShader::TextShader(SDL_GPUDevice* device, SDL_Window* window, bool& success) {
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

	VertexAttributeDefinition attribDefs[] = {
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4},
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4},
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2}
	};
	success = init(
		device,
		window,
		{.src = textRenderVShaderSrc.data(),
		 .vertexAttribDefs = attribDefs,
		 .srcLen = sizeof(textRenderVShaderSrc),
		 .vertBufferSize = BL_TEXT_VERTEX_BUF_SIZE * sizeof(Vertex),
		 .indexBufferSize = BL_TEXT_INDEX_BUF_SIZE * sizeof(int32_t),
		 .vertexAttribCount = 3,
		 .uniformBufferCount = 1},
		{.src = textRenderFShaderSrc.data(),
		 .srcLen = sizeof(textRenderFShaderSrc),
		 .samplerCount = 1}
	);
}

bool TextShader::queueTextSequence(const Colour& color, TTF_GPUAtlasDrawSequence* sequence) {
	bool isDataFree = true;
	size_t vertCount = size_t(sequence->num_vertices);
	size_t indexCount = size_t(sequence->num_indices);
	if (vertCount > BL_TEXT_VERTEX_BUF_SIZE - geometryData.vertexCount) {
		vertCount = BL_TEXT_VERTEX_BUF_SIZE - geometryData.vertexCount;
		isDataFree = false;
	}
	if (indexCount > BL_TEXT_INDEX_BUF_SIZE - geometryData.indexCount) {
		indexCount = BL_TEXT_INDEX_BUF_SIZE - geometryData.indexCount;
		isDataFree = false;
	}

	for (size_t i = 0; i < vertCount; i++) {
		const SDL_FPoint pos = sequence->xy[i];
		Vertex vert = {.pos = {pos.x, pos.y, 0.0f, 1.0f}, .color = color, .uv = sequence->uv[i]};
		geometryData.vertices[geometryData.vertexCount + i] = vert;
	}
	SDL_memcpy(
		&geometryData.indices[geometryData.indexCount],
		sequence->indices,
		indexCount * sizeof(int32_t)
	);

	geometryData.vertexCount += vertCount;
	geometryData.indexCount += indexCount;
	return isDataFree;
}

void TextShader::setGeometryData() const {
	Vertex* transferData = (Vertex*) SDL_MapGPUTransferBuffer(device, vertTransBuf, false);
	SDL_memcpy(
		transferData,
		geometryData.vertices.data(),
		sizeof(Vertex) * geometryData.vertexCount
	);
	SDL_memcpy(
		transferData + BL_TEXT_VERTEX_BUF_SIZE,
		geometryData.indices.data(),
		sizeof(int32_t) * geometryData.indexCount
	);
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
}

void TextShader::onResize(uint32_t width, uint32_t height) {
	matrices[0] = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 1.0f);
}

void TextShader::pushText(
	TTF_Text* textObject,
	const Vector2& pos,
	float height,
	const Colour& color,
	size_t clipIndex
) {
	if (textCount == BL_MAX_TEXT_COUNT) {
		return;
	}
	float scale = height / BL_FONT_SIZE;
	glm::mat4 transform = {1.0f};
	transform = glm::translate(transform, {pos.x, pos.y, 0.0f});
	transform = glm::scale(transform, {scale, scale, 1.0f});
	data[textCount] =
		{.transform = transform, .textObject = textObject, .color = color, .clipIndex = clipIndex};
	textCount++;
}

void TextShader::clearData() {
	textCount = 0;
	geometryData.vertexCount = 0;
	geometryData.indexCount = 0;
}

void TextShader::prepareShader() {
	if (textCount == 0) {
		return;
	}

	for (size_t i = 0; i < textCount; i++) {
		TTF_GPUAtlasDrawSequence* sequence = TTF_GetGPUTextDrawData(data[i].textObject);
		for (; sequence != nullptr; sequence = sequence->next) {
			if (!queueTextSequence(data[i].color, sequence)) {
				goto exitLoop;
			}
		}
	}

exitLoop:
	// Get the text data and queue the text in a buffer for drawing later
	setGeometryData();
}

void TextShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (textCount == 0) {
		return;
	}
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = sizeof(Vertex) * BL_TEXT_VERTEX_BUF_SIZE
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

void TextShader::runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) {
	if (textCount == 0) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

	int32_t vertexOffset = 0, indexOffset = 0;
	for (size_t i = 0; i < textCount; i++) {
		TextData textData = data[i];
		matrices[1] = textData.transform;
		SDL_SetGPUScissor(renderPass, &app->renderState.clipRects[textData.clipIndex]);
		SDL_PushGPUVertexUniformData(cmdbuf, 0, matrices.data(), sizeof(matrices));
		TTF_GPUAtlasDrawSequence* seq = TTF_GetGPUTextDrawData(textData.textObject);
		for (; seq != nullptr; seq = seq->next) {
			SDL_GPUTextureSamplerBinding samplerBinding = {
				.texture = seq->atlas_texture,
				.sampler = sampler
			};

			bool shouldBreak = false;
			int32_t vertCount = int32_t(seq->num_vertices);
			int32_t indexCount = int32_t(seq->num_indices);
			if (vertCount > BL_TEXT_VERTEX_BUF_SIZE - int32_t(vertexOffset)) {
				vertCount = BL_TEXT_VERTEX_BUF_SIZE - int32_t(vertexOffset);
				shouldBreak = true;
			}
			if (indexCount > BL_TEXT_INDEX_BUF_SIZE - int32_t(indexOffset)) {
				indexCount = BL_TEXT_INDEX_BUF_SIZE - int32_t(indexOffset);
				shouldBreak = true;
			}

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
	clearData();
	SDL_ReleaseGPUSampler(device, sampler);
}
