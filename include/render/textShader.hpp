#pragma once

#include "../../extern/glm/gtc/matrix_transform.hpp"  // IWYU pragma: keep
#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "../datatype/vector2.hpp"
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>

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
	};

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

	TextData data[BL_MAX_TEXT_COUNT] = {};

	glm::mat4 matrices[2] = {{1.0f}, {1.0f}};

	SDL_GPUSampler* sampler;

	size_t textCount = 0;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool queueTextSequence(const Color& color, TTF_GPUAtlasDrawSequence* sequence);

	void setGeometryData() const;

  public:
	TextShader(
		SDL_GPUDevice* device,
		SDL_Window* window,
		TTF_TextEngine* textEngine,
		bool& success
	);

	void onResize(uint32_t width, uint32_t height) override;

	void pushText(
		const char* str,
		Font* font,
		const Vector2& pos,
		float height,
		const Color& color
	);
	void clearData() override;

	void prepareShader() override;
	void runCopyPass(const SDL_GPUCommandBuffer* cmdbuf, SDL_GPUCopyPass* copyPass) override;
	void runRenderPass(
		SDL_GPUCommandBuffer* cmdbuf,
		SDL_GPURenderPass* renderPass,
		SDL_GPUTexture* swapTex
	) override;

	~TextShader();
};

}  // namespace Bloodlust
