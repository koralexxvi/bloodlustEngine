#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"
#include "window.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/gtc/matrix_transform.hpp>

#define BL_TEXT_VERTEX_BUF_INIT_SIZE (100)
#define BL_TEXT_INDEX_BUF_INIT_SIZE (144)

namespace Bloodlust {

// The internal shader responsible for drawing text.
class TextShader : public ShaderProgram {
  public:
	// Creates a text shader and returns true on success.
	TextShader(SDL_GPUDevice* device, Window* window, bool& success);

	// A callback fired when the window is resized.
	void onResize(std::uint32_t width, std::uint32_t height) override;

	// Returns the amount of text objects queued to be drawn.
	constexpr std::size_t getQueued() const {
		return 0;
	}

	// Preallocates enough memory to hold the given number of queued text data.
	void reserve(std::size_t count);

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
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	// Destroys this shader.
	~TextShader();
};

}  // namespace Bloodlust
