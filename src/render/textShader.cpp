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
		 .vertBufferSize = BL_TEXT_VERTEX_BUF_INIT_SIZE,
		 .indexBufferSize = BL_TEXT_INDEX_BUF_INIT_SIZE,
		 .vertexAttribCount = 3,
		 .uniformBufferCount = 1},
		{.src = textRenderFShaderSrc.data(),
		 .srcLen = sizeof(textRenderFShaderSrc),
		 .samplerCount = 1}
	);
}

void TextShader::queueTextSequence(const Colour& colour, TTF_GPUAtlasDrawSequence* sequence) {
	std::size_t vertCount = std::size_t(sequence->num_vertices);
	std::size_t indexCount = std::size_t(sequence->num_indices);
	vertices.reserve(vertices.size() + vertCount);
	indices.reserve(indices.size() + indexCount);
	for (std::size_t i = 0; i < vertCount; i++) {
		const SDL_FPoint pos = sequence->xy[i];
		Vertex vert = {.pos = {pos.x, pos.y, 0.0f, 1.0f}, .colour = colour, .uv = sequence->uv[i]};
		vertices.push_back(vert);
	}
	for (std::size_t i = 0; i < indexCount; i++) {
		indices.push_back(std::uint32_t(sequence->indices[i]));
	}
}

void TextShader::onResize(std::uint32_t width, std::uint32_t height) {
	matrices[0] = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 1.0f);
}

std::size_t TextShader::getQueued() const {
	return data.size();
}

void TextShader::reserve(std::size_t count) {
	data.reserve(count);
}

void TextShader::pushText(
	TTF_Text* textObject,
	const Vector2& pos,
	float height,
	const Colour& colour,
	std::size_t clipIndex
) {
	float scale = height / BL_FONT_SIZE;
	glm::mat4 transform = {1.0f};
	transform = glm::translate(transform, {pos.x, pos.y, 0.0f});
	transform = glm::scale(transform, {scale, scale, 1.0f});
	data.push_back(
		{.transform = transform, .textObject = textObject, .colour = colour, .clipIndex = clipIndex}
	);
}

void TextShader::clearData() {
	data.clear();
	vertices.clear();
	indices.clear();
	// TODO: improve memory management here as this is really slow
	data.shrink_to_fit();
	vertices.shrink_to_fit();
	indices.shrink_to_fit();
}

bool TextShader::prepareShader() {
	if (data.empty()) {
		return true;
	}
	for (const auto& v : data) {
		TTF_GPUAtlasDrawSequence* sequence = TTF_GetGPUTextDrawData(v.textObject);
		for (; sequence != nullptr; sequence = sequence->next) {
			queueTextSequence(v.colour, sequence);
		}
	}

	if (!(resizeVertexBuffer(std::uint32_t(vertices.size() * sizeof(Vertex)))
		  && resizeIndexBuffer(std::uint32_t(indices.size() * sizeof(std::uint32_t)))
		  && resizeTransferBuffer()))
	{
		return false;
	}

	char* transferData = static_cast<char*>(SDL_MapGPUTransferBuffer(device, vertTransBuf, false));
	SDL_memcpy(transferData, vertices.data(), vertices.size() * sizeof(Vertex));
	SDL_memcpy(
		transferData + (vertices.size() * sizeof(Vertex)),
		indices.data(),
		sizeof(std::uint32_t) * indices.size()
	);
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
	return true;
}

void TextShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (data.empty()) {
		return;
	}
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = std::uint32_t(vertices.size() * sizeof(Vertex))
	};
	SDL_GPUBufferRegion vertRegion = {
		.buffer = vertBuffer,
		.offset = 0,
		.size = std::uint32_t(vertices.size() * sizeof(Vertex))
	};
	SDL_GPUBufferRegion indexRegion = {
		.buffer = indexBuffer,
		.offset = 0,
		.size = std::uint32_t(indices.size() * sizeof(std::uint32_t))
	};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
}

void TextShader::runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) {
	if (data.empty()) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

	std::uint32_t vertexOffset = 0, indexOffset = 0;
	for (const auto& textData : data) {
		matrices[1] = textData.transform;
		SDL_SetGPUScissor(renderPass, &app->renderState.clipRects[textData.clipIndex]);
		SDL_PushGPUVertexUniformData(cmdbuf, 0, matrices.data(), sizeof(matrices));
		TTF_GPUAtlasDrawSequence* seq = TTF_GetGPUTextDrawData(textData.textObject);
		for (; seq != nullptr; seq = seq->next) {
			std::uint32_t vertCount = std::uint32_t(seq->num_vertices);
			std::uint32_t indexCount = std::uint32_t(seq->num_indices);
			SDL_GPUTextureSamplerBinding samplerBinding = {
				.texture = seq->atlas_texture,
				.sampler = sampler
			};
			SDL_BindGPUFragmentSamplers(renderPass, 0, &samplerBinding, 1);
			SDL_DrawGPUIndexedPrimitives(
				renderPass,
				indexCount,
				1,
				indexOffset,
				std::int32_t(vertexOffset),
				0
			);
			vertexOffset += vertCount;
			indexOffset += indexCount;
		}
	}
}

TextShader::~TextShader() {
	clearData();
	SDL_ReleaseGPUSampler(device, sampler);
}
