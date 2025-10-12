#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"
#include "window.hpp"

#include <SDL3/SDL_gpu.h>
#include <glm/mat4x4.hpp>
#include <vector>

#define BL_TEXTURE_VERTEX_BUF_INIT_SIZE (1440)

namespace Bloodlust {

// The internal shader responsible for drawing textures.
class TextureShader : public ShaderProgram {
  private:
	using TextureVertData = std::array<float, 6>;

	// A struct representing queued texture data.
	struct TextureData {
		// The model's transformation matrix.
		glm::mat4 model;

		// A pointer to the texture.
		const Texture* texture;

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
	std::vector<std::array<TextureVertData, 4>> vertexData = {};

	// The matrix data.
	std::array<glm::mat4, 2> matrices = {{1.0f}};

	// The sampler used for drawing textures.
	SDL_GPUSampler* sampler;

  public:
	// Creates a texture shader and returns true on success.
	TextureShader(SDL_GPUDevice* device, Window* window, bool& success);

	// A callback fired when the window is resized.
	void onResize(std::uint32_t width, std::uint32_t height) override;

	// Returns the amount of textures queued to be drawn.
	constexpr std::size_t getQueued() const {
		return textures.size();
	}

	// Preallocates enough memory to hold the given number of queued texture data.
	void reserve(std::size_t count);

	// Queues a texture to be drawn in the next render step.
	void pushTexture(
		const Texture* tex,
		const Vector2& pos,
		const Vector2& size,
		const Colour& colour,
		const SDL_FRect& clipRect,
		std::size_t clipIndex
	);

	// Clears the queued texture data.
	void clearData() override;

	// Prepares this shader for a draw operation.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	// Destroys this shader.
	~TextureShader();
};

}  // namespace Bloodlust
