#include "../../include/render/rectFillShader.hpp"

#include "../../extern/glm/gtc/matrix_transform.hpp"  // IWYU pragma: keep

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

using Bloodlust::RectFillShader;

namespace {

constexpr float rectVertices[] = {
	// clang-format off
	1.0f, 1.0f, 0.0f, 1.0f,  // top right
	0.0f, 1.0f, 0.0f, 1.0f,  // top left
	0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
	1.0f, 0.0f, 0.0f, 1.0f   // bottom right
	// clang-format on
};
constexpr uint32_t rectIndices[] = {0, 1, 3, 1, 2, 3};

// TODO: replace embed with a cpp20 compliant equivalent
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wc23-extensions"
#endif
constexpr unsigned char vertSrc[] = {
#embed "../../res/shader/compiled/primitiveFill.vert.spv"
};

constexpr unsigned char fragSrc[] = {
#embed "../../res/shader/compiled/solidColor.frag.spv"
};
#ifdef __clang__
	#pragma clang diagnostic pop
#endif

}  // namespace

RectFillShader::RectFillShader(SDL_GPUDevice* device, SDL_Window* window, bool& success) {
	SDL_GPUBufferCreateInfo bufferInfo = {
		.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
		.size = sizeof(rectBufferData),
		.props = 0
	};
	storageBuffer = SDL_CreateGPUBuffer(device, &bufferInfo);
	if (storageBuffer == nullptr) {
		success = false;
		return;
	}
	SDL_GPUTransferBufferCreateInfo transBufInfo =
		{.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = sizeof(rectBufferData), .props = 0};
	storageTransBuf = SDL_CreateGPUTransferBuffer(device, &transBufInfo);
	if (storageTransBuf == nullptr) {
		success = false;
		return;
	}

	VertexAttributeDefinition attribDef = {SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4};
	success = init(
		device,
		window,
		{.src = vertSrc,
		 .vertexAttribDefs = &attribDef,
		 .srcLen = sizeof(vertSrc),
		 .vertBufferSize = sizeof(rectVertices),
		 .indexBufferSize = sizeof(rectIndices),
		 .vertexAttribCount = 1,
		 .storageBufferCount = 1,
		 .uniformBufferCount = 1},
		{.src = fragSrc, .srcLen = sizeof(fragSrc)}
	);
}

void RectFillShader::pushRect(const SDL_FRect& rect, const Color& color, float zIndex) {
	if (rectCount == BL_MAX_RECTFILL_COUNT) {
		return;
	}
	glm::mat4 transform = {1.0f};
	transform = glm::translate(transform, {rect.x, rect.y, float(zIndex)});
	transform = glm::scale(transform, {rect.w, rect.h, 1.0f});
	rectBufferData[rectCount] = {.transform = transform, .color = color};
	rectCount++;
}

void RectFillShader::clearData() {
	rectCount = 0;
}

void RectFillShader::onResize(uint32_t width, uint32_t height) {
	projection = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 1.0f);
	// projection = glm::perspective(glm::radians(150.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	// projection = glm::translate(projection, {-640.0f, -360.0f, -60.0f});
}

void RectFillShader::prepareShader() {
	if (rectCount == 0) {
		return;
	}
	char* vertData = (char*) SDL_MapGPUTransferBuffer(device, vertTransBuf, false);
	SDL_memcpy(vertData, rectVertices, sizeof(rectVertices));
	SDL_memcpy(vertData + sizeof(rectVertices), rectIndices, sizeof(rectIndices));
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
	RectData* rectData = (RectData*) SDL_MapGPUTransferBuffer(device, storageTransBuf, false);
	SDL_memcpy(rectData, rectBufferData, rectCount * sizeof(RectData));
	SDL_UnmapGPUTransferBuffer(device, storageTransBuf);
}

void RectFillShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (rectCount == 0) {
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
	SDL_GPUBufferRegion storageRegion =
		{.buffer = storageBuffer, .offset = 0, .size = uint32_t(rectCount * sizeof(RectData))};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &storageTransLoc, &storageRegion, false);
}

void RectFillShader::runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) {
	if (rectCount == 0) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
	SDL_BindGPUVertexStorageBuffers(renderPass, 0, &storageBuffer, 1);
	SDL_PushGPUVertexUniformData(cmdbuf, 0, &projection, sizeof(projection));
	SDL_DrawGPUIndexedPrimitives(renderPass, 6, uint32_t(rectCount), 0, 0, 0);
}

RectFillShader::~RectFillShader() {
	SDL_ReleaseGPUBuffer(device, storageBuffer);
	SDL_ReleaseGPUTransferBuffer(device, storageTransBuf);
}
