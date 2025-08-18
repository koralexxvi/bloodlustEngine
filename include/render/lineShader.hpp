#pragma once

#include "../datatype/color.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <glm/mat4x4.hpp>

#define BL_MAX_LINE_COUNT (8192)

namespace Bloodlust {

class LineShader : public ShaderProgram {
  private:
	struct LineData {
		glm::vec3 pos;
		Color color;
	};

	std::array<std::array<LineData, 4>, BL_MAX_LINE_COUNT> lineData = {};
	std::array<size_t, BL_MAX_LINE_COUNT> clipRects = {};

	glm::mat4 projection;
	size_t lineCount = 0;

  public:
	LineShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	void onResize(uint32_t width, uint32_t height) override;
	void clearData() override;

	void pushLine(
		const Vector2& p1,
		const Vector2& p2,
		float width,
		const Color& color,
		size_t clipIndex
	);

	void prepareShader() override;
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;
};

}  // namespace Bloodlust
