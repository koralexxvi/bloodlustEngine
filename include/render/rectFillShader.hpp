#pragma once

#include "../../extern/glm/mat4x4.hpp"	// IWYU pragma: keep
#include "../datatype/color.hpp"
#include "../render/shaderProgram.hpp"

#include <SDL3/SDL_video.h>

#define BL_MAX_RECTFILL_COUNT (8192)

namespace Bloodlust {

class RectFillShader : public ShaderProgram {
  private:
	struct RectData {
		glm::mat4 transform;
		Color color;
	};

	RectData rectBufferData[BL_MAX_RECTFILL_COUNT] = {};
	glm::mat4 projection;
	SDL_GPUBuffer* storageBuffer;
	SDL_GPUTransferBuffer* storageTransBuf;

	size_t rectCount = 0;

  public:
	RectFillShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	void pushRect(const SDL_FRect& rect, const Color& color);
	void clearData() override;

	void onResize(uint32_t width, uint32_t height) override;
	void prepareShader() override;
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	~RectFillShader();
};

}  // namespace Bloodlust
