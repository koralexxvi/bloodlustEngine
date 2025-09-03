#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <glm/mat4x4.hpp>
#include <vector>

#define BL_LINE_VERTEX_BUF_INIT_ELEMENTS (64)

namespace Bloodlust {

// The internal shader responsible for drawing lines.
class LineShader : public ShaderProgram {
  private:
	// A struct containing the data for a line point.
	struct LineData {
		// The position of the vertex.
		glm::vec4 pos;

		// The colour of the vertex.
		Colour colour;
	};

	using LineVertices = std::array<LineData, 4>;

	// The vector containing the data for each line's vertices.
	std::vector<LineVertices> lineData = {};

	// The array containing the clip rect for each line.
	std::vector<std::size_t> clipRects = {};

	// The projection matrix for this shader.
	glm::mat4 projection;

  public:
	// Creates a line shader and returns true on success.
	LineShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	// A callback fired when the window is resized.
	void onResize(std::uint32_t width, std::uint32_t height) override;

	// Clears the queued data.
	void clearData() override;

	// Queues a line to be drawn for the next render pass.
	void pushLine(
		const Vector2& p1,
		const Vector2& p2,
		float width,
		const Colour& colour,
		std::size_t clipIndex
	);

	// Prepares this shader for a draw operation.
	void prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;
};

}  // namespace Bloodlust
