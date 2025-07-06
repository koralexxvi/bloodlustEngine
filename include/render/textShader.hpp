#pragma once

#include "../../extern/glm/gtc/matrix_transform.hpp"  // IWYU pragma: keep
#include "../datatype/color.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>

#define BL_MAX_TEXT_COUNT (8'192)
#define BL_TEXT_VERTEX_BUF_SIZE (196'612)
#define BL_TEXT_INDEX_BUF_SIZE (294'918)

#define FONT_PATH "./res/testFont.ttf"

namespace Bloodlust {

struct TextInfo {
	Color color = {1.0f, 1.0f, 1.0f};
	const char* str;
	Vector2 pos;
	float size;

	[[maybe_unused]]
	char _padding[4] = {};
};

class TextShader : public ShaderProgram {
  private:
	struct Vertex {
		glm::vec3 pos;
		Color color;
		SDL_FPoint uv;
	};

	struct {
		Vertex vertices[BL_TEXT_VERTEX_BUF_SIZE];
		int indices[BL_TEXT_INDEX_BUF_SIZE];
		size_t vertexCount = 0;
		size_t indexCount = 0;
	} geometryData = {};

	TextInfo data[BL_MAX_TEXT_COUNT] = {};
	TTF_Text* textObjects[BL_MAX_TEXT_COUNT] = {};

	glm::mat4 matrices[2] = {{1.0f}, {1.0f}};

	SDL_GPUSampler* sampler;
	TTF_Font* font;

	uint32_t textCount = 0;

	[[maybe_unused]]
	char _padding[4];

	bool queueTextSequence(const TextInfo& textInfo, TTF_GPUAtlasDrawSequence* sequence);

	void setGeometryData();

  public:
	TextShader(
		SDL_GPUDevice* device,
		SDL_Window* window,
		TTF_TextEngine* textEngine,
		bool& success
	);

	void onResize(uint32_t width, uint32_t height) override;

	void pushText(const TextInfo& textData);
	void clearData() override;

	void prepareShader() override;
	void runCopyPass(SDL_GPUCommandBuffer* cmdbuf, SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(
		SDL_GPUCommandBuffer* cmdbuf,
		SDL_GPURenderPass* renderPass,
		SDL_GPUTexture* swapTex
	) override;

	~TextShader();
};

}  // namespace Bloodlust
