#include "../../include/render/lineShader.hpp"

#include "../../extern/glm/gtc/matrix_transform.hpp"  // IWYU pragma: keep

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

namespace {

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wc23-extensions"
#endif
constexpr unsigned char vertSrc[] = {
#embed "../../res/shader/compiled/lineFill.vert.spv"
};

constexpr unsigned char fragSrc[] = {
#embed "../../res/shader/compiled/solidColor.frag.spv"
};
#ifdef __clang__
	#pragma clang diagnostic pop
#endif

constexpr uint32_t indices[] = {0, 1, 3, 1, 2, 3};

}  // namespace

using Bloodlust::LineShader;

LineShader::LineShader(SDL_GPUDevice* device, SDL_Window* window, bool& success) {
	VertexAttributeDefinition attribDefs[] = {
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3},
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4}
	};
	success = init(
		device,
		window,
		{.src = vertSrc,
		 .vertexAttribDefs = attribDefs,
		 .srcLen = sizeof(vertSrc),
		 .vertBufferSize = sizeof(lineData),
		 .indexBufferSize = sizeof(indices),
		 .vertexAttribCount = 2},
		{.src = fragSrc, .srcLen = sizeof(fragSrc)}
	);
}

void LineShader::onResize(uint32_t width, uint32_t height) {
	projection = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.1f, 100.0f);
}

void LineShader::clearData() {
	lineCount = 0;
}

void LineShader::pushLine(const Vector2& p1, const Vector2& p2, float width, const Color& color) {
	if (lineCount == BL_MAX_LINE_COUNT) {
		return;
	}
	Vector2 normal = Vector2(p1.y - p2.y, p2.x - p1.x).normalize() * (width * 0.5f);
	Vector2 r1 = p1 - normal, r2 = p1 + normal, r3 = p2 - normal, r4 = p2 + normal;
	lineData[lineCount][0] = {glm::vec3(r1.x, r1.y, 0.0f), color};
	lineData[lineCount][1] = {glm::vec3(r2.x, r2.y, 0.0f), color};
	lineData[lineCount][2] = {glm::vec3(r3.x, r3.y, 0.0f), color};
	lineData[lineCount][3] = {glm::vec3(r4.x, r4.y, 0.0f), color};
	lineCount++;
}

void LineShader::prepareShader() {
	if (lineCount == 0) {
		return;
	}
	char* vertData = (char*) SDL_MapGPUTransferBuffer(device, vertTransBuf, false);
	SDL_memcpy(vertData, lineData, sizeof(lineData));
	SDL_memcpy(vertData + sizeof(lineData), indices, sizeof(indices));
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
}

void LineShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (lineCount == 0) {
		return;
	}
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = sizeof(lineData)
	};
	SDL_GPUBufferRegion vertRegion = {.buffer = vertBuffer, .offset = 0, .size = sizeof(lineData)};
	SDL_GPUBufferRegion indexRegion = {.buffer = indexBuffer, .offset = 0, .size = sizeof(indices)};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
}

void LineShader::runRenderPass(SDL_GPUCommandBuffer*, SDL_GPURenderPass* renderPass) {
	if (lineCount == 0) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
	for (size_t i = 0; i < lineCount; i++) {
		SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);
	}
}
