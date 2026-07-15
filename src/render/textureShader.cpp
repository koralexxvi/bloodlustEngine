#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/asset/texture.hpp>
#include <bloodlustEngine/exceptions/internalError.hpp>
#include <bloodlustEngine/logging.hpp>
#include <bloodlustEngine/render/shaderSrc/textureDraw.frag.hpp>
#include <bloodlustEngine/render/shaderSrc/textureDraw.vert.hpp>
#include <bloodlustEngine/render/shaderUtil.hpp>
#include <bloodlustEngine/render/textureShader.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {

constexpr std::array<std::uint32_t, 6> indices = {0, 1, 3, 1, 2, 3};

}  // namespace

using Bloodlust::TextureShader;

TextureShader::TextureShader(SDL_GPUDevice* device, Window* window) {
	SDL_GPUSamplerCreateInfo samplerInfo = {};
	samplerInfo.min_filter = SDL_GPU_FILTER_NEAREST;
	samplerInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
	samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
	samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	nearestSampler = SDL_CreateGPUSampler(device, &samplerInfo);
	if (nearestSampler == nullptr) {
		throw InternalError("Could not create texture sampler (nearest): " + getError());
	}
	samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
	samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
	samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
	linearSampler = SDL_CreateGPUSampler(device, &samplerInfo);
	if (linearSampler == nullptr) {
		throw InternalError("Could not create texture sampler (linear): " + getError());
	}

	VertexAttributeDefinition attribDefs[] = {
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4},
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2}
	};
	init(
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

void TextureShader::onResize(std::uint32_t width, std::uint32_t height) noexcept {
	projection =
		glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.0f, 1.0f);
}

void TextureShader::pushTexture(
	const Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const Colour& colour,
	const SDL_FRect& clipRect,
	std::size_t clipIndex
) noexcept {
	Vector2 texSize = texture->getSize();
	SDL_FRect uvRect = {
		clipRect.x / texSize.x,
		1.0f - (clipRect.y / texSize.y),
		clipRect.w / texSize.x,
		clipRect.h / texSize.y
	};
	vertexData.reserve(vertexData.size() * 4);
	// top right
	vertexData.push_back(
		{pos.x + size.x, pos.y + size.y, 0.0f, 1.0f, uvRect.x + uvRect.w, uvRect.y}
	);
	// top left
	vertexData.push_back({pos.x, pos.y + size.y, 0.0f, 1.0f, uvRect.x, uvRect.y});
	// bottom left
	vertexData.push_back({pos.x, pos.y, 0.0f, 1.0f, uvRect.x, uvRect.y - uvRect.h});
	// bottom right
	vertexData.push_back(
		{pos.x + size.x, pos.y, 0.0f, 1.0f, uvRect.x + uvRect.w, uvRect.y - uvRect.h}
	);
	textures.push_back(
		{.texture = texture, .colour = colour, .uvRect = uvRect, .clipIndex = clipIndex}
	);
}

void TextureShader::reserve(std::size_t count) noexcept {
	vertexData.reserve(count);
	textures.reserve(count);
}

void TextureShader::clearData() noexcept {
	textures.clear();
	vertexData.clear();
	// TODO: improve memory management here as this is really slow
	textures.shrink_to_fit();
	vertexData.shrink_to_fit();
}

void TextureShader::prepareShader() {
	if (textures.empty()) {
		return;
	}
	std::size_t vertDataSize = vertexData.size() * sizeof(TextureVertData);
	std::size_t indexDataSize = vertexData.size() * sizeof(indices);
	resizeVertexBuffer(static_cast<std::uint32_t>(vertDataSize));
	resizeIndexBuffer(static_cast<std::uint32_t>(indexDataSize));
	resizeTransferBuffer();
	char* vertPtr = static_cast<char*>(SDL_MapGPUTransferBuffer(device, vertTransBuf, false));
	SDL_memcpy(vertPtr, vertexData.data(), vertDataSize);
	std::array<std::uint32_t, 6> arr = indices;
	for (std::size_t i = 0; i < indexDataSize; i += sizeof(arr)) {
		SDL_memcpy(vertPtr + (vertDataSize + i), arr.data(), sizeof(arr));
		for (auto&& v : arr) {
			v += 4;
		}
	}
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
}

void TextureShader::runCopyPass(SDL_GPUCopyPass* copyPass) noexcept {
	if (textures.empty()) {
		return;
	}
	std::size_t vertDataSize = vertexData.size() * sizeof(TextureVertData);
	std::size_t indexDataSize = vertexData.size() * sizeof(indices);
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = static_cast<std::uint32_t>(vertDataSize)
	};
	SDL_GPUBufferRegion vertRegion =
		{.buffer = vertBuffer, .offset = 0, .size = static_cast<std::uint32_t>(vertDataSize)};
	SDL_GPUBufferRegion indexRegion =
		{.buffer = indexBuffer, .offset = 0, .size = static_cast<std::uint32_t>(indexDataSize)};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
}

void TextureShader::runRenderPass(
	SDL_GPUCommandBuffer* cmdbuf,
	SDL_GPURenderPass* renderPass
) noexcept {
	if (textures.empty()) {
		return;
	}
	SDL_GPUTextureSamplerBinding samplerBinding;
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	std::uint32_t indexOffset = 0;
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
	SDL_PushGPUVertexUniformData(cmdbuf, 0, &projection, sizeof(glm::mat4));
	for (const auto& texData : textures) {
		samplerBinding.texture = texData.texture->getData();
		samplerBinding.sampler = texData.texture->getFilterMode() == TextureFilterMode::NEAREST
								   ? nearestSampler
								   : linearSampler;
		SDL_SetGPUScissor(renderPass, &app->renderState.clipRects[texData.clipIndex]);
		SDL_PushGPUFragmentUniformData(cmdbuf, 0, &texData.colour, sizeof(Colour));
		SDL_BindGPUFragmentSamplers(renderPass, 0, &samplerBinding, 1);
		SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, indexOffset, 0, 0);
		indexOffset += 6;
	}
}

TextureShader::~TextureShader() noexcept {
	SDL_ReleaseGPUSampler(device, nearestSampler);
	SDL_ReleaseGPUSampler(device, linearSampler);
}
