#pragma once

#include "../../extern/glm/mat4x4.hpp"	// IWYU pragma: keep
#include "../datatype/color.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>

#define BL_MAX_TEXTURE_COUNT (8192)

namespace Bloodlust {

class TextureShader : public ShaderProgram {
  private:
	struct TextureData {
		glm::mat4 model;
		const Texture* texture;
		Color color;
		size_t clipIndex;
	};

	TextureData textures[BL_MAX_TEXTURE_COUNT] = {};
	glm::mat4 matrices[2] = {{1.0f}, {1.0f}};
	SDL_GPUSampler* sampler;

	size_t textureCount = 0;

  public:
	TextureShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	void onResize(uint32_t width, uint32_t height) override;

	void pushTexture(
		const Texture* tex,
		const Vector2& pos,
		const Vector2& size,
		const Color& color,
		size_t clipIndex
	);
	void clearData() override;

	void prepareShader() override;
	void runCopyPass(const SDL_GPUCommandBuffer* cmdbuf, SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(
		SDL_GPUCommandBuffer* cmdbuf,
		SDL_GPURenderPass* renderPass,
		SDL_GPUTexture* swapTex
	) override;

	~TextureShader();
};

}  // namespace Bloodlust
