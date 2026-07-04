#pragma once

#include "../util.hpp"
#include "shaderUtil.hpp"
#include "window.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <cstdint>

namespace Bloodlust {

// A class representing a shader program.
class ShaderProgram {
  protected:
	// A pointer to the GPU context.
	SDL_GPUDevice* device;

	// A pointer to the window.
	Window* window;

	// A pointer to the graphics pipeline.
	SDL_GPUGraphicsPipeline* pipeline = nullptr;

	// A pointer to the vertex buffer.
	SDL_GPUBuffer* vertBuffer = nullptr;

	// A pointer to the index buffer.
	SDL_GPUBuffer* indexBuffer = nullptr;

	// A pointer to the transfer buffer containing the data for both the vertex and index buffers.
	SDL_GPUTransferBuffer* vertTransBuf = nullptr;

	// The current size of the vertex buffer.
	std::uint32_t vertBufferSize = 0;

	// The current size of the index buffer.
	std::uint32_t indexBufferSize = 0;

	// The current size of the transfer buffer.
	std::uint32_t transBufSize = 0;

  private:
	[[maybe_unused]]
	char _padding[4];

  protected:
	// Initialises this shader program and returns true on success.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool init(
		SDL_GPUDevice* device,
		Window* window,
		const VertShaderDef& vertDef,
		const FragShaderDef& fragDef
	) noexcept;

	// Resizes the vertex buffer to fit its elements. The size must be greater than 0, and the
	// contents of the new buffer will be uninitialised.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool resizeVertexBuffer(std::uint32_t size) noexcept;

	// Resizes the index buffer to fit its elements. The size must be greater than 0, and the
	// contents of the new buffer will be uninitialised.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool resizeIndexBuffer(std::uint32_t size) noexcept;

	// Resizes the vertex/index transfer buffer to the sum of the size of the vertex and index
	// buffers. The contents of the new buffer will be uninitialised.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool resizeTransferBuffer() noexcept;

  public:
	// A callback that is fired when the window is resized.
	virtual void onResize(std::uint32_t width, std::uint32_t height) noexcept = 0;

	// Clears this shader program's queued data.
	virtual void clearData() noexcept = 0;

	// Prepares this shader for a draw operation.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool prepareShader() noexcept = 0;

	// Runs this shader's copy pass.
	virtual void runCopyPass(SDL_GPUCopyPass* copyPass) noexcept = 0;

	// Runs this shader's render pass.
	virtual void runRenderPass(
		SDL_GPUCommandBuffer* cmdbuf,
		SDL_GPURenderPass* renderPass
	) noexcept = 0;

	virtual ~ShaderProgram() noexcept;
};

}  // namespace Bloodlust
