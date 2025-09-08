#include "../../include/render/textureShader.hpp"

#include "../../include/app.hpp"
#include "../../include/render/shaderSrc/textureDraw.frag.hpp"
#include "../../include/render/shaderSrc/textureDraw.vert.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {

// clang-format off
constexpr std::array<float, 24> vertexData = {
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
	0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top left
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
	1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f   // bottom right
};
// clang-format on
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
		window->getHandle(),
		{.src = textureDrawVShaderSrc.data(),
		 .vertexAttribDefs = attribDefs,
		 .srcLen = sizeof(textureDrawVShaderSrc),
		 .vertBufferSize = sizeof(vertexData),
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
	matrices[0] = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 1.0f);
}

void TextureShader::pushTexture(
	const Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const Colour& colour,
	std::size_t clipIndex
) {
	glm::mat4 model = {1.0f};
	model = glm::translate(model, {pos.x, pos.y, 0.0f});
	model = glm::scale(model, {size.x, size.y, 1.0f});
	textures.push_back(
		{.model = model, .texture = texture, .colour = colour, .clipIndex = clipIndex}
	);
}

std::size_t TextureShader::getQueued() const {
	return textures.size();
}

void TextureShader::reserve(std::size_t count) {
	textures.reserve(count);
}

void TextureShader::clearData() {
	textures.clear();
	// TODO: improve memory management here as this is really slow
	textures.shrink_to_fit();
}

bool TextureShader::prepareShader() {
	if (textures.empty()) {
		return true;
	}
	char* vertPtr = static_cast<char*>(SDL_MapGPUTransferBuffer(device, vertTransBuf, false));
	SDL_memcpy(vertPtr, vertexData.data(), sizeof(vertexData));
	SDL_memcpy(vertPtr + sizeof(vertexData), indices.data(), sizeof(indices));
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
	return true;
}

void TextureShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (textures.empty()) {
		return;
	}
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = sizeof(vertexData)
	};
	SDL_GPUBufferRegion vertRegion =
		{.buffer = vertBuffer, .offset = 0, .size = sizeof(vertexData)};
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
