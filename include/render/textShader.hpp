#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define BL_TEXT_VERTEX_BUF_INIT_SIZE (100)
#define BL_TEXT_INDEX_BUF_INIT_SIZE (144)

namespace Bloodlust {

// The internal shader responsible for drawing text.
class TextShader : public ShaderProgram {
  private:
	// A structure representing a text object to be drawn.
	struct TextData {
		// The transformation matrix of this text object.
		glm::mat4 transform;

		// The internal text object.
		TTF_Text* textObject;

		// The colour of the text object.
		Colour colour;

		// The index of the clip rect, or 0 for no clipping.
		std::size_t clipIndex;
	};

	// A structure representing a vertex to be drawn.
	struct Vertex {
		// The position of this vertex.
		glm::vec4 pos;

		// The colour of this vertex.
		Colour colour;

		// The texture coordinate of this vertex.
		SDL_FPoint uv;
	};

	// A vector of the vertices to be drawn.
	std::vector<Vertex> vertices = {};

	// A vector of the indices to be drawn.
	std::vector<std::uint32_t> indices = {};

	// An array of the queued text data to be drawn.
	std::vector<TextData> data = {};

	std::array<glm::mat4, 2> matrices = {{1.0f}};

	// The sampler used for drawing from the font atlas.
	SDL_GPUSampler* sampler;

	// Queues the geometry data of this text sequence.
	void queueTextSequence(const Colour& colour, TTF_GPUAtlasDrawSequence* sequence);

  public:
	// Creates a text shader and returns true on success.
	TextShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	// A callback fired when the window is resized.
	void onResize(std::uint32_t width, std::uint32_t height) override;

	// Queues a text object for drawing in the next render step.
	void pushText(
		TTF_Text* textObject,
		const Vector2& pos,
		float height,
		const Colour& colour,
		std::size_t clipIndex
	);

	// Clears the queued data.
	void clearData() override;

	// Prepares this shader for a draw operation.
	void prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	// Destroys this shader.
	~TextShader();
};

}  // namespace Bloodlust
