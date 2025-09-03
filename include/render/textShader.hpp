#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/gtc/matrix_transform.hpp>

#define BL_MAX_TEXT_COUNT (256)
#define BL_TEXT_VERTEX_BUF_SIZE (6148)
#define BL_TEXT_INDEX_BUF_SIZE (9220)

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

	// The geometry data of the text objects to be drawn.
	struct {
		// An array of vertices to be drawn.
		std::array<Vertex, BL_TEXT_VERTEX_BUF_SIZE> vertices = {};

		// An array of indices to be drawn.
		std::array<std::uint32_t, BL_TEXT_INDEX_BUF_SIZE> indices = {};

		// The amount of vertices to be drawn.
		std::size_t vertexCount = 0;

		// The amount of indices to be drawn.
		std::size_t indexCount = 0;
	} geometryData = {};

	// An array of the queued text data to be drawn.
	std::array<TextData, BL_MAX_TEXT_COUNT> data = {};

	std::array<glm::mat4, 2> matrices = {{1.0f}};

	// The sampler used for drawing from the font atlas.
	SDL_GPUSampler* sampler;

	// The amount of text data structs queued.
	std::size_t textCount = 0;

	// Queues the geometry data of this text sequence.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool queueTextSequence(const Colour& colour, TTF_GPUAtlasDrawSequence* sequence);

	// Copies the geometry data of the queued vertices to the transfer buffer.
	void setGeometryData() const;

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
