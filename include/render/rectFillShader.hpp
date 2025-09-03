#pragma once

#include "../datatype/colour.hpp"
#include "../render/shaderProgram.hpp"

#include <SDL3/SDL_video.h>
#include <glm/mat4x4.hpp>

#define BL_MAX_RECTFILL_COUNT (256)

namespace Bloodlust {

// The internal shader responsible for drawing rectangles.
class RectFillShader : public ShaderProgram {
  private:
	// A struct containing the data for each rect.
	struct RectData {
		// The transformation matrix to be applied.
		glm::mat4 transform;

		// The colour of the rect.
		Colour colour;
	};

	// An array containing the data for each rect to be drawn.
	std::array<RectData, BL_MAX_RECTFILL_COUNT> rectBufferData = {};

	// The projection matrix for this shader.
	glm::mat4 projection;

	// The storage buffer containing data for each rect to be drawn.
	SDL_GPUBuffer* storageBuffer;

	// The transfer buffer for the shader's storage buffer.
	SDL_GPUTransferBuffer* storageTransBuf;

	// The amount of rects queued to be drawn.
	std::size_t rectCount = 0;

  public:
	// Creates a rect shader and returns true on success.
	RectFillShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	// Queues a rect to be drawn.
	void pushRect(const SDL_FRect& rect, const Colour& colour);

	// Clears the queued data.
	void clearData() override;

	// A callback fired when the window is resized.
	void onResize(std::uint32_t width, std::uint32_t height) override;

	// Prepares this shader for a draw operation.
	void prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	// Destroys this shader.
	~RectFillShader();
};

}  // namespace Bloodlust
