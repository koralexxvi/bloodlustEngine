#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <glm/mat4x4.hpp>

#define BL_MAX_TEXTURE_COUNT (8192)

namespace Bloodlust {

// The internal shader responsible for drawing textures.
class TextureShader : public ShaderProgram {
  private:
	// A struct representing queued texture data.
	struct TextureData {
		// The model's transformation matrix.
		glm::mat4 model;

		// A pointer to the texture.
		const Texture* texture;

		// The colour of the texture.
		Colour color;

		// The clip index of the texture, or 0 for no clipping.
		std::size_t clipIndex;
	};

	// The queued texture data.
	std::array<TextureData, BL_MAX_TEXTURE_COUNT> textures = {};

	// The matrix data.
	std::array<glm::mat4, 2> matrices = {{1.0f}};

	// The sampler used for drawing textures.
	SDL_GPUSampler* sampler;

	// The amount of textures queued to be drawn.
	std::size_t textureCount = 0;

  public:
	// Creates a texture shader and returns true on success.
	TextureShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	// A callback fired when the window is resized.
	void onResize(std::uint32_t width, std::uint32_t height) override;

	// Queues a texture to be drawn in the next render step.
	void pushTexture(
		const Texture* tex,
		const Vector2& pos,
		const Vector2& size,
		const Colour& color,
		std::size_t clipIndex
	);

	// Clears the queued texture data.
	void clearData() override;

	// Prepares this shader for a draw operation.
	void prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	// Destroys this shader.
	~TextureShader();
};

}  // namespace Bloodlust
