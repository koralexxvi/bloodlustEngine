#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <glm/mat4x4.hpp>

#define BL_MAX_LINE_COUNT (8192)

namespace Bloodlust {

// The internal shader responsible for drawing lines.
class LineShader : public ShaderProgram {
  private:
	// A struct containing the data for a line point.
	struct LineData {
		// The position of the vertex.
		glm::vec4 pos;

		// The colour of the vertex.
		Colour color;
	};

	// The array containing the data for each line's vertices.
	std::array<std::array<LineData, 4>, BL_MAX_LINE_COUNT> lineData = {};

	// The array containing the clip rect for each line.
	std::array<size_t, BL_MAX_LINE_COUNT> clipRects = {};

	// The projection matrix for this shader.
	glm::mat4 projection;

	// The amount of lines queued to be drawn.
	size_t lineCount = 0;

  public:
	// Creates a line shader and returns true on success.
	LineShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	// A callback fired when the window is resized.
	void onResize(uint32_t width, uint32_t height) override;

	// Clears the queued data.
	void clearData() override;

	// Queues a line to be drawn for the next render pass.
	void pushLine(
		const Vector2& p1,
		const Vector2& p2,
		float width,
		const Colour& color,
		size_t clipIndex
	);

	// Prepares this shader for a draw operation.
	void prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;
};

}  // namespace Bloodlust
