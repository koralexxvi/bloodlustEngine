#pragma once

#include "../../extern/glm/gtc/matrix_transform.hpp"  // IWYU pragma: keep
#include "shaderProgram.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>

#define MAX_VERTEX_COUNT (196'612)
#define MAX_INDEX_COUNT (294'918)

#define FONT_PATH "./res/testFont.ttf"

namespace Bloodlust {

class TextShader : public ShaderProgram {
	struct Vertex {
		glm::vec3 pos;
		SDL_FColor color;
		SDL_FPoint uv;
	};

	struct GeometryData {
		Vertex vertices[MAX_VERTEX_COUNT];
		int indices[MAX_INDEX_COUNT];
		int vertexCount = 0;
		int indexCount = 0;
	};

  private:
	GeometryData geometryData = {};

	glm::mat4 matrices[2] = {
		glm::perspective(glm::radians(100.0f), 16.0f / 9.0f, 0.1f, 200.0f),
		{1.0f}
	};
	SDL_FColor color = {1.0f, 1.0f, 0.0f, 1.0f};

	SDL_GPUSampler* sampler;
	TTF_Text* text;
	TTF_Font* font;
	TTF_GPUAtlasDrawSequence* sequence;

	uint64_t lastDraw;
	float rotAngle = 0.0f;
	char str[20] = "     \nTEST STRING T";

	void queueTextSequence(TTF_GPUAtlasDrawSequence* sequence);
	void queueText();

	void setGeometryData();

  public:
	TextShader(
		SDL_GPUDevice* device,
		SDL_Window* window,
		TTF_TextEngine* textEngine,
		bool& success
	);

	void onResize(uint32_t width, uint32_t height) override;

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
