#pragma once

#include "../datatype/color.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/gtc/matrix_transform.hpp>

#define BL_MAX_TEXT_COUNT (8192)
#define BL_TEXT_VERTEX_BUF_SIZE (196'612)
#define BL_TEXT_INDEX_BUF_SIZE (294'918)

namespace Bloodlust {

class TextShader : public ShaderProgram {
  private:
	struct TextData {
		glm::mat4 transform;
		TTF_Text* textObject;
		Color color;
		size_t clipIndex;
	};

	struct Vertex {
		glm::vec3 pos;
		Color color;
		SDL_FPoint uv;
	};

	struct {
		std::array<Vertex, BL_TEXT_VERTEX_BUF_SIZE> vertices = {};
		std::array<int, BL_TEXT_INDEX_BUF_SIZE> indices = {};
		size_t vertexCount = 0;
		size_t indexCount = 0;
	} geometryData = {};

	std::array<TextData, BL_MAX_TEXT_COUNT> data = {};

	std::array<glm::mat4, 2> matrices = {{1.0f}};

	SDL_GPUSampler* sampler;

	size_t textCount = 0;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool queueTextSequence(const Color& color, TTF_GPUAtlasDrawSequence* sequence);

	void setGeometryData() const;

  public:
	TextShader(SDL_GPUDevice* device, SDL_Window* window, bool& success);

	void onResize(uint32_t width, uint32_t height) override;

	void pushText(
		TTF_Text* textObject,
		const Vector2& pos,
		float height,
		const Color& color,
		size_t clipIndex
	);
	void clearData() override;

	void prepareShader() override;
	void runCopyPass(SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPURenderPass* renderPass) override;

	~TextShader();
};

}  // namespace Bloodlust
