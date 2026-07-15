#pragma once

#include "../datatype/colour.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <glm/mat4x4.hpp>
#include <vector>

#define BL_TEXTURE_VERTEX_BUF_INIT_SIZE (1440)

namespace Bloodlust {

struct Vector2;
class Texture;
class Window;

// The internal shader responsible for drawing textures.
class TextureShader : public ShaderProgram {
  private:
	using TextureVertData = std::array<float, 6>;

	// A struct representing queued texture data.
	struct TextureData {
		// A pointer to the texture.
		const Texture& texture;

		// The colour of the texture.
		Colour colour;

		// The UV rect of the texture.
		SDL_FRect uvRect;

		// The clip index of the texture, or 0 for no clipping.
		std::size_t clipIndex;
	};

	// The queued texture data.
	std::vector<TextureData> textures = {};

	// The queued vertex data.
	std::vector<TextureVertData> vertexData = {};

	// The matrix data.
	glm::mat4 projection = {1.0f};

	// The sampler used for drawing textures with nearest neighbour filtering.
	SDL_GPUSampler* nearestSampler;

	// The sampler used for drawing textures with linear filtering.
	SDL_GPUSampler* linearSampler;

  public:
	// Creates a texture shader. Will throw InternalError on failure.
	TextureShader(SDL_GPUDevice* device, Window* window);

	// A callback fired when the window is resized.
	void onResize(std::uint32_t width, std::uint32_t height) noexcept override;

	// Returns the amount of textures queued to be drawn.
	constexpr std::size_t getQueued() const noexcept {
		return textures.size();
	}

	// Preallocates enough memory to hold the given number of queued texture data.
	void reserve(std::size_t count) noexcept;

	// Queues a texture to be drawn in the next render step.
	void pushTexture(
		const Texture& tex,
		const Vector2& pos,
		const Vector2& size,
		const Colour& colour,
		const SDL_FRect& clipRect,
		std::size_t clipIndex
	) noexcept;

	// Clears the queued texture data.
	void clearData() noexcept override;

	// Prepares this shader for a draw operation. Will throw InternalError on failure.
	void prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) noexcept override;

	// Runs this shader's render pass.
	void runRenderPass(
		SDL_GPUCommandBuffer* cmdbuf,
		SDL_GPURenderPass* renderPass
	) noexcept override;

	// Destroys this shader.
	~TextureShader() noexcept;
};

}  // namespace Bloodlust
