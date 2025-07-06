#pragma once

#include "../../extern/glm/mat4x4.hpp"	// IWYU pragma: keep
#include "../../extern/glm/vec3.hpp"	// IWYU pragma: keep
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>

#define BL_MAX_TEXTURE_COUNT (16'384)

namespace Bloodlust {

class TextureShader : public ShaderProgram {
  private:
	struct TextureData {
		Texture* texture;
		glm::mat4 model;
	};

	TextureData textures[BL_MAX_TEXTURE_COUNT] = {};
	glm::mat4 matrices[2] = {{1.0f}, {1.0f}};
	SDL_GPUSampler* sampler;

	uint32_t textureCount = 0;

	[[maybe_unused]]
	char _padding[4];

  public:
	TextureShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	void onResize(uint32_t width, uint32_t height) override;

	void pushTexture(Texture* tex, const Vector2& pos, const Vector2& size);
	void clearData() override;

	void prepareShader() override;
	void runCopyPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(
		SDL_GPUCommandBuffer* cmdbuf,
		SDL_GPURenderPass* renderPass,
		SDL_GPUTexture* swapTex
	) override;

	~TextureShader();
};

}  // namespace Bloodlust
