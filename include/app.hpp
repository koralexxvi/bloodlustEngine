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

#define BL_MAX_CLIP_COUNT (1024)

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

		SDL_Rect clipRects[BL_MAX_CLIP_COUNT] = {};
		int clipCount = 0;

		[[maybe_unused]]
		char _padding[4];
	} renderState;

	std::unique_ptr<Scene> currentScene = nullptr;

	SDL_Window* window;

	uint64_t lastDraw = SDL_GetTicksNS();
	uint64_t lastStep = SDL_GetTicksNS();

	bool changeScene(Scene* newScene);

	AppResult init(int argcount, char* args[]);
	AppResult step();
	AppResult draw(uint64_t dt);

	bool processInput(const InputEvent& event);
};

extern App* app;

}  // namespace Bloodlust
