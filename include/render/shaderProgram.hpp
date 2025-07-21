#pragma once

#include "../util.hpp"
#include "shaderUtil.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

namespace Bloodlust {

class ShaderProgram {
  protected:
	SDL_GPUDevice* device;
	SDL_Window* window;
	SDL_GPUGraphicsPipeline* pipeline = nullptr;

	SDL_GPUBuffer* vertBuffer = nullptr;
	SDL_GPUBuffer* indexBuffer = nullptr;
	SDL_GPUTransferBuffer* vertTransBuf = nullptr;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool init(
		SDL_GPUDevice* device,
		SDL_Window* window,
		const VertShaderDef& vertDef,
		const FragShaderDef& fragDef
	);

  public:
	virtual void onResize(uint32_t width, uint32_t height) = 0;
	virtual void clearData() = 0;

	virtual void prepareShader() = 0;
	virtual void runCopyPass(SDL_GPUCopyPass* copyPass) = 0;
	virtual void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) = 0;

	virtual ~ShaderProgram();
};

}  // namespace Bloodlust
