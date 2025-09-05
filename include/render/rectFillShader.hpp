#pragma once

#include "../datatype/colour.hpp"
#include "../render/shaderProgram.hpp"

#include <SDL3/SDL_video.h>
#include <glm/mat4x4.hpp>
#include <vector>

#define BL_RECTFILL_STRANSBUF_INIT_SIZE (1280)

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
	std::vector<RectData> rectBufferData = {};

	// The projection matrix for this shader.
	glm::mat4 projection;

	// The storage buffer containing data for each rect to be drawn.
	SDL_GPUBuffer* storageBuffer = nullptr;

	// The transfer buffer for the shader's storage buffer.
	SDL_GPUTransferBuffer* storageTransBuf = nullptr;

	// The current size of the storage buffer.
	std::size_t storageBufSize = 0;

	// Resizes the storage buffer and its transfer buffer to the specified size.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool resizeStorageBuffers(std::uint32_t size);

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
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool prepareShader() override;

	// Runs this shader's copy pass.
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;

	// Runs this shader's render pass.
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	// Destroys this shader.
	~RectFillShader();
};

}  // namespace Bloodlust
