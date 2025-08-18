#pragma once

#include "datatype/inputEvent.hpp"
#include "datatype/soundChannel.hpp"
#include "render/lineShader.hpp"
#include "render/rectFillShader.hpp"
#include "render/textShader.hpp"
#include "render/textureShader.hpp"
#include "scene.hpp"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <memory>

#ifndef BL_MAX_CLIP_COUNT
	#define BL_MAX_CLIP_COUNT (8192)
#endif

#ifndef BL_AUDIO_STREAM_COUNT
	#define BL_AUDIO_STREAM_COUNT (8)
#endif

namespace Bloodlust {

struct App {
	struct RenderState {
		SDL_GPUDevice* device;
		TTF_TextEngine* textEngine;
		SDL_GPUTexture* depthTexture;

		RectFillShader* rectFillShader;
		TextureShader* textureShader;
		TextShader* textShader;
		LineShader* lineShader;

		std::array<SDL_Rect, BL_MAX_CLIP_COUNT> clipRects = {};
		size_t clipCount = 0;
	} renderState;

	struct AudioState {
		std::array<SoundChannel*, BL_AUDIO_STREAM_COUNT> channels = {};
		SDL_AudioDeviceID deviceOut = 0;

	  private:
		[[maybe_unused]]
		char _padding[4];
	} audioState;

	std::unique_ptr<Scene> currentScene = nullptr;

	SDL_Window* window;

	uint64_t lastDraw = SDL_GetTicksNS();
	uint64_t lastStep = SDL_GetTicksNS();

  public:
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult changeScene(Scene* newScene);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult init(int argcount, char* args[]);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult step();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult draw(uint64_t dt);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult processInput(const InputEvent& event);
};

extern App* app;

}  // namespace Bloodlust
