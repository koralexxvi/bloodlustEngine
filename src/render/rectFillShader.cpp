#include "../../include/render/rectFillShader.hpp"

#include "../../include/render/shaderSrc/primitiveFill.vert.hpp"
#include "../../include/render/shaderSrc/solidColour.frag.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <glm/gtc/matrix_transform.hpp>

using Bloodlust::RectFillShader;

namespace {

constexpr std::array<float, 16> rectVertices = {
	// clang-format off
	1.0f, 1.0f, 0.0f, 1.0f,  // top right
	0.0f, 1.0f, 0.0f, 1.0f,  // top left
	0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
	1.0f, 0.0f, 0.0f, 1.0f   // bottom right
	// clang-format on
};
constexpr std::array<std::uint32_t, 6> rectIndices = {0, 1, 3, 1, 2, 3};

}  // namespace

bool RectFillShader::resizeStorageBuffers(std::uint32_t size) {
	if (size == storageBufSize) {
		return true;
	}
	storageBufSize = size;
	SDL_ReleaseGPUBuffer(device, storageBuffer);
	SDL_ReleaseGPUTransferBuffer(device, storageTransBuf);
	SDL_GPUBufferCreateInfo bufferInfo =
		{.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, .size = size, .props = 0};
	SDL_GPUTransferBufferCreateInfo transBufInfo =
		{.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = size, .props = 0};
	storageBuffer = SDL_CreateGPUBuffer(device, &bufferInfo);
	storageTransBuf = SDL_CreateGPUTransferBuffer(device, &transBufInfo);
	return storageBuffer != nullptr && storageTransBuf != nullptr;
}

RectFillShader::RectFillShader(SDL_GPUDevice* device, SDL_Window* window, bool& success) {
	VertexAttributeDefinition attribDef = {SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4};
	success = init(
		device,
		window,
		{.src = primitiveFillVShaderSrc.data(),
		 .vertexAttribDefs = &attribDef,
		 .srcLen = sizeof(primitiveFillVShaderSrc),
		 .vertBufferSize = sizeof(rectVertices),
		 .indexBufferSize = sizeof(rectIndices),
		 .vertexAttribCount = 1,
		 .storageBufferCount = 1,
		 .uniformBufferCount = 1},
		{.src = solidColourFShaderSrc.data(), .srcLen = sizeof(solidColourFShaderSrc)}
	);
	if (success) {
		success = resizeStorageBuffers(BL_RECTFILL_STRANSBUF_INIT_SIZE);
	}
}

void RectFillShader::pushRect(const SDL_FRect& rect, const Colour& colour) {
	glm::mat4 transform = {1.0f};
	transform = glm::translate(transform, {rect.x, rect.y, 0.0f});
	transform = glm::scale(transform, {rect.w, rect.h, 1.0f});
	rectBufferData.push_back({.transform = transform, .colour = colour});
}

void RectFillShader::clearData() {
	rectBufferData.clear();
	// TODO: improve memory management here as this is really slow
	rectBufferData.shrink_to_fit();
}

void RectFillShader::onResize(std::uint32_t width, std::uint32_t height) {
	projection = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 1.0f);
}

bool RectFillShader::prepareShader() {
	if (rectBufferData.empty()) {
		return true;
	}

	if (!resizeStorageBuffers(std::uint32_t(rectBufferData.size() * sizeof(RectData)))) {
		return false;
	}

	char* vertData = static_cast<char*>(SDL_MapGPUTransferBuffer(device, vertTransBuf, false));
	SDL_memcpy(vertData, rectVertices.data(), sizeof(rectVertices));
	SDL_memcpy(vertData + sizeof(rectVertices), rectIndices.data(), sizeof(rectIndices));
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
	RectData* rectData =
		static_cast<RectData*>(SDL_MapGPUTransferBuffer(device, storageTransBuf, false));
	SDL_memcpy(rectData, rectBufferData.data(), rectBufferData.size() * sizeof(RectData));
	SDL_UnmapGPUTransferBuffer(device, storageTransBuf);
	return true;
}

void RectFillShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (rectBufferData.empty()) {
		return;
	}
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = sizeof(rectVertices)
	};
	SDL_GPUTransferBufferLocation storageTransLoc = {
		.transfer_buffer = storageTransBuf,
		.offset = 0
	};
	SDL_GPUBufferRegion vertRegion =
		{.buffer = vertBuffer, .offset = 0, .size = sizeof(rectVertices)};
	SDL_GPUBufferRegion indexRegion =
		{.buffer = indexBuffer, .offset = 0, .size = sizeof(rectIndices)};
	SDL_GPUBufferRegion storageRegion = {
		.buffer = storageBuffer,
		.offset = 0,
		.size = std::uint32_t(rectBufferData.size() * sizeof(RectData))
	};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &storageTransLoc, &storageRegion, false);
}

void RectFillShader::runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) {
	if (rectBufferData.empty()) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
	SDL_BindGPUVertexStorageBuffers(renderPass, 0, &storageBuffer, 1);
	SDL_PushGPUVertexUniformData(cmdbuf, 0, &projection, sizeof(projection));
	SDL_DrawGPUIndexedPrimitives(
		renderPass,
		6,
		std::uint32_t(rectBufferData.size() * sizeof(RectData)),
		0,
		0,
		0
	);
}

RectFillShader::~RectFillShader() {
	SDL_ReleaseGPUBuffer(device, storageBuffer);
	SDL_ReleaseGPUTransferBuffer(device, storageTransBuf);
}
