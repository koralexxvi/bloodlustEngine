#pragma once

#include "../../extern/glm/mat4x4.hpp"	// IWYU pragma: keep
#include "../datatype/color.hpp"
#include "../datatype/vector2.hpp"
#include "../render/shaderProgram.hpp"

#include <SDL3/SDL_video.h>

#define BL_MAX_RECTFILL_COUNT (8192)

namespace Bloodlust {

struct RectData {
	glm::mat4 transform;
	glm::vec4 color;
};

class RectFillShader : public ShaderProgram {
  private:
	RectData rectBufferData[BL_MAX_RECTFILL_COUNT] = {};
	glm::mat4 projection;
	SDL_GPUBuffer* storageBuffer;
	SDL_GPUTransferBuffer* storageTransBuf;

	uint32_t rectCount = 0;

	[[maybe_unused]]
	char _padding[4];

  public:
	RectFillShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	void pushRect(const Vector2& pos, const Vector2& size, const Color& color);
	void clearData() override;

	void onResize(uint32_t width, uint32_t height) override;
	void prepareShader() override;
	void runCopyPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(
		SDL_GPUCommandBuffer* cmdbuf,
		SDL_GPURenderPass* renderPass,
		SDL_GPUTexture* swapTex
	) override;

	~RectFillShader();
};

}  // namespace Bloodlust
