#pragma once

#include "../util.hpp"
#include "shaderUtil.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

namespace Bloodlust {

// A class representing a shader program.
class ShaderProgram {
  protected:
	// A pointer to the GPU context.
	SDL_GPUDevice* device;

	// A pointer to the window struct.
	SDL_Window* window;

	// A pointer to the graphics pipeline.
	SDL_GPUGraphicsPipeline* pipeline = nullptr;

	// A pointer to the vertex buffer.
	SDL_GPUBuffer* vertBuffer = nullptr;

	// A pointer to the index buffer.
	SDL_GPUBuffer* indexBuffer = nullptr;

	// A pointer to the transfer buffer containing the data for both the vertex and index buffers.
	SDL_GPUTransferBuffer* vertTransBuf = nullptr;

	// Initialises this shader program and returns true on success.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool init(
		SDL_GPUDevice* device,
		SDL_Window* window,
		const VertShaderDef& vertDef,
		const FragShaderDef& fragDef
	);

  public:
	// A callback that is fired when the window is resized.
	virtual void onResize(uint32_t width, uint32_t height) = 0;

	// Clears this shader program's queued data.
	virtual void clearData() = 0;

	// Prepares this shader for a draw operation.
	virtual void prepareShader() = 0;

	// Runs this shader's copy pass.
	virtual void runCopyPass(SDL_GPUCopyPass* copyPass) = 0;

	// Runs this shader's render pass.
	virtual void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) = 0;

	virtual ~ShaderProgram();
};

}  // namespace Bloodlust
