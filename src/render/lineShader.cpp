#include "../../include/render/lineShader.hpp"

#include "../../include/app.hpp"
#include "../../include/render/shaderSrc/lineFill.vert.hpp"
#include "../../include/render/shaderSrc/solidColour.frag.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {

constexpr std::array<std::uint32_t, 6> indices = {0, 1, 3, 1, 2, 3};

}  // namespace

using Bloodlust::LineShader;

LineShader::LineShader(SDL_GPUDevice* device, Window* window, bool& success) {
	VertexAttributeDefinition attribDefs[] = {
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4},
		{SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4}
	};
	success = init(
		device,
		window,
		{.src = lineFillVShaderSrc.data(),
		 .vertexAttribDefs = attribDefs,
		 .srcLen = sizeof(lineFillVShaderSrc),
		 .vertBufferSize = BL_LINE_VERTEX_BUF_INIT_ELEMENTS * sizeof(LineVertices),
		 .indexBufferSize = sizeof(indices),
		 .vertexAttribCount = 2,
		 .uniformBufferCount = 1},
		{.src = solidColourFShaderSrc.data(), .srcLen = sizeof(solidColourFShaderSrc)}
	);
}

void LineShader::onResize(std::uint32_t width, std::uint32_t height) {
	projection = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 1.0f);
}

std::size_t LineShader::getQueued() const {
	return lineData.size();
}

void LineShader::reserve(std::size_t count) {
	lineData.reserve(count);
}

void LineShader::clearData() {
	clipRects.clear();
	lineData.clear();
	// TODO: improve memory management here as this is really slow
	clipRects.shrink_to_fit();
	lineData.shrink_to_fit();
}

void LineShader::pushLine(
	const Vector2& p1,
	const Vector2& p2,
	float width,
	const Colour& colour,
	std::size_t clipIndex
) {
	if (p1 == p2) {
		return;
	}
	Vector2 normal = Vector2(p2.y - p1.y, p1.x - p2.x).normalize() * (width * 0.5f);
	Vector2 r1 = p1 - normal, r2 = p1 + normal, r3 = p2 + normal, r4 = p2 - normal;
	clipRects.push_back(clipIndex);
	lineData.push_back(
		{LineData{.pos = {r1.x, r1.y, 0.0f, 1.0f}, .colour = colour},
		 LineData{.pos = {r2.x, r2.y, 0.0f, 1.0f}, .colour = colour},
		 LineData{.pos = {r3.x, r3.y, 0.0f, 1.0f}, .colour = colour},
		 LineData{.pos = {r4.x, r4.y, 0.0f, 1.0f}, .colour = colour}}
	);
}

bool LineShader::prepareShader() {
	if (lineData.empty()) {
		return true;
	}
	if (!(resizeVertexBuffer(std::uint32_t(lineData.size() * sizeof(LineVertices)))
		  && resizeTransferBuffer()))
	{
		return false;
	}
	char* vertData = static_cast<char*>(SDL_MapGPUTransferBuffer(device, vertTransBuf, false));
	SDL_memcpy(vertData, lineData.data(), lineData.size() * sizeof(LineVertices));
	SDL_memcpy(
		vertData + (lineData.size() * sizeof(LineVertices)),
		indices.data(),
		sizeof(indices)
	);
	SDL_UnmapGPUTransferBuffer(device, vertTransBuf);
	return true;
}

void LineShader::runCopyPass(SDL_GPUCopyPass* copyPass) {
	if (lineData.empty()) {
		return;
	}
	SDL_GPUTransferBufferLocation vertTransLoc = {.transfer_buffer = vertTransBuf, .offset = 0};
	SDL_GPUTransferBufferLocation indexTransLoc = {
		.transfer_buffer = vertTransBuf,
		.offset = std::uint32_t(lineData.size() * sizeof(LineVertices))
	};
	SDL_GPUBufferRegion vertRegion = {
		.buffer = vertBuffer,
		.offset = 0,
		.size = std::uint32_t(lineData.size() * sizeof(LineVertices))
	};
	SDL_GPUBufferRegion indexRegion = {.buffer = indexBuffer, .offset = 0, .size = sizeof(indices)};
	SDL_UploadToGPUBuffer(copyPass, &vertTransLoc, &vertRegion, false);
	SDL_UploadToGPUBuffer(copyPass, &indexTransLoc, &indexRegion, false);
}

void LineShader::runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) {
	if (lineData.empty()) {
		return;
	}
	SDL_GPUBufferBinding vertBinding = {.buffer = vertBuffer, .offset = 0};
	SDL_GPUBufferBinding indexBinding = {.buffer = indexBuffer, .offset = 0};
	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertBinding, 1);
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
	SDL_PushGPUVertexUniformData(cmdbuf, 1, &projection, sizeof(glm::mat4));
	for (std::size_t i = 0; i < lineData.size(); i++) {
		SDL_SetGPUScissor(renderPass, &app->renderState.clipRects[clipRects[i]]);
		SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, std::int32_t(i * 4), 0);
	}
}
