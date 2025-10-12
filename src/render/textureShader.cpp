#include "../../include/render/textureShader.hpp"

#include "../../include/app.hpp"
#include "../../include/render/shaderSrc/textureDraw.frag.hpp"
#include "../../include/render/shaderSrc/textureDraw.vert.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {

constexpr std::array<std::uint32_t, 6> indices = {0, 1, 3, 1, 2, 3};

}  // namespace

using Bloodlust::TextureShader;

TextureShader::TextureShader(SDL_GPUDevice* device, Window* window, bool& success) {
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
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2}
	};
	success = init(
		device,
		window,
		{.src = textureDrawVShaderSrc.data(),
		 .vertexAttribDefs = attribDefs,
		 .srcLen = sizeof(textureDrawVShaderSrc),
		 .vertBufferSize = BL_TEXTURE_VERTEX_BUF_INIT_SIZE,
		 .indexBufferSize = sizeof(indices),
		 .vertexAttribCount = 2,
		 .uniformBufferCount = 1},
		{.src = textureDrawFShaderSrc.data(),
		 .srcLen = sizeof(textureDrawFShaderSrc),
		 .samplerCount = 1,
		 .uniformBufferCount = 1}
	);
}

void TextureShader::onResize(std::uint32_t width, std::uint32_t height) {
	matrices[0] =
		glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.0f, 1.0f);
}

void TextureShader::pushTexture(
	const Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const Colour& colour,
	const SDL_FRect& clipRect,
	std::size_t clipIndex
) {
	glm::mat4 model = {1.0f};
	Vector2 texSize = texture->getSize();
	SDL_FRect uvRect = {
		clipRect.x / texSize.x,
		1.0f - (clipRect.y / texSize.y),
		clipRect.w / texSize.x,
		clipRect.h / texSize.y
	};
	model = glm::translate(model, {pos.x, pos.y, 0.0f});
	model = glm::scale(model, {size.x, size.y, 1.0f});
	vertexData.reserve(vertexData.size() * 4);
	// top right
	vertexData.push_back({1.0f, 1.0f, 0.0f, 1.0f, uvRect.x + uvRect.w, uvRect.y});
	// top left
	vertexData.push_back({0.0f, 1.0f, 0.0f, 1.0f, uvRect.x, uvRect.y});
	// bottom left
	vertexData.push_back({0.0f, 0.0f, 0.0f, 1.0f, uvRect.x, uvRect.y - uvRect.h});
	// bottom right
	vertexData.push_back({1.0f, 0.0f, 0.0f, 1.0f, uvRect.x + uvRect.w, uvRect.y - uvRect.h});
	textures.push_back(
		{.model = model,
		 .texture = texture,
		 .colour = colour,
		 .uvRect = uvRect,
		 .clipIndex = clipIndex}
	);
}

void TextureShader::reserve(std::size_t count) {
	vertexData.reserve(count);
	textures.reserve(count);
}

void TextureShader::clearData() {
	textures.clear();
	vertexData.clear();
	// TODO: improve memory management here as this is really slow
	textures.shrink_to_fit();
	vertexData.shrink_to_fit();
}

bool TextureShader::prepareShader() {
	if (textures.empty()) {
		return true;
	}
	std::size_t vertDataSize = vertexData.size() * sizeof(TextureVertData);
	if (!(resizeVertexBuffer(static_cast<std::uint32_t>(vertDataSize)) && resizeTransferBuffer())) {
		return false;
	}
	char* vertPtr = static_cast<char*>(SDL_MapGPUTransferBuffer(device, vertTransBuf, false));
	SDL_memcpy(vertPtr, vertexData.data(), vertDataSize);
	SDL_memcpy(vertPtr + vertDataSize, indices.data(), sizeof(indices));
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
	return true;
}

void TextureShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (textures.empty()) {
		return;
	}
	std::size_t vertDataSize = vertexData.size() * sizeof(TextureVertData);
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = static_cast<std::uint32_t>(vertDataSize)
	};
	SDL_GPUBufferRegion vertRegion =
		{.buffer = vertBuffer, .offset = 0, .size = static_cast<std::uint32_t>(vertDataSize)};
	SDL_GPUBufferRegion indexRegion = {.buffer = indexBuffer, .offset = 0, .size = sizeof(indices)};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
}

void TextureShader::runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) {
	if (textures.empty()) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_GPUTextureSamplerBinding samplerBinding = {.texture = nullptr, .sampler = sampler};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
	for (const auto& texData : textures) {
		samplerBinding.texture = texData.texture->getData();
		matrices[1] = texData.model;
		SDL_SetGPUScissor(renderPass, &app->renderState.clipRects[texData.clipIndex]);
		SDL_PushGPUVertexUniformData(cmdbuf, 0, matrices.data(), sizeof(matrices));
		SDL_PushGPUFragmentUniformData(cmdbuf, 0, &texData.colour, sizeof(Colour));
		SDL_BindGPUFragmentSamplers(renderPass, 0, &samplerBinding, 1);
		SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);
	}
}

TextureShader::~TextureShader() {
	SDL_ReleaseGPUSampler(device, sampler);
}
