#pragma once

#include "datatype/inputEvent.hpp"
#include "render/rectFillShader.hpp"
#include "render/textShader.hpp"
#include "render/textureShader.hpp"
#include "scene.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <memory>

namespace Bloodlust {

enum class AppResult {
	CONTINUE,
	SUCCESS,
	FAILURE
};

struct App {
	struct RenderState {
		SDL_GPUDevice* device;
		TTF_TextEngine* textEngine;
		SDL_GPUTexture* depthTexture;

		RectFillShader* rectFillShader;
		TextureShader* textureShader;
		TextShader* textShader;
	} renderState;

	std::unique_ptr<Scene> currentScene = nullptr;

	SDL_Window* window;

	uint64_t lastDraw = SDL_GetTicksNS();
	uint64_t lastStep = SDL_GetTicksNS();

	bool changeScene(Scene* newScene);

	virtual AppResult init(int argcount, char* args[]) = 0;
	virtual AppResult step() = 0;
	virtual AppResult draw(uint64_t dt) = 0;

	virtual bool processInput(const InputEvent& event) = 0;

	virtual ~App() = default;
};

extern App* app;

}  // namespace Bloodlust
