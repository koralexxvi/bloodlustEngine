#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <glm/mat4x4.hpp>

#define BL_MAX_TEXTURE_COUNT (8192)

namespace Bloodlust {

class TextureShader : public ShaderProgram {
  private:
	struct TextureData {
		glm::mat4 model;
		const Texture* texture;
		Colour color;
		size_t clipIndex;
	};

	std::array<TextureData, BL_MAX_TEXTURE_COUNT> textures = {};
	std::array<glm::mat4, 2> matrices = {{1.0f}};
	SDL_GPUSampler* sampler;

	size_t textureCount = 0;

  public:
	TextureShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	void onResize(uint32_t width, uint32_t height) override;

	void pushTexture(
		const Texture* tex,
		const Vector2& pos,
		const Vector2& size,
		const Colour& color,
		size_t clipIndex
	);
	void clearData() override;

	void prepareShader() override;
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	~TextureShader();
};

}  // namespace Bloodlust
