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

// The global state for the running app.
struct App {
	// A struct that contains the renderer's state.
	struct RenderState {
		// A pointer to the GPU device handle.
		SDL_GPUDevice* device;

		// A pointer to the internal text engine used for drawing glyphs.
		TTF_TextEngine* textEngine;

		// A pointer to the depth texture.
		SDL_GPUTexture* depthTexture;

		// The internal rect fill shader.
		RectFillShader* rectFillShader;

		// The internal texture shader.
		TextureShader* textureShader;

		// The internal text shader.
		TextShader* textShader;

		// The internal line shader.
		LineShader* lineShader;

		// An array of clip rects to be used by the internal shaders.
		std::array<SDL_Rect, BL_MAX_CLIP_COUNT> clipRects = {};

		// The amount of clip rects queued.
		size_t clipCount = 0;
	};

	// A struct that contains the audio mixer's state.
	struct AudioState {
		// An array of the available sound channels. The amount of channels is defined by
		// BL_AUDIO_STREAM_COUNT and defaults to 8.
		std::array<SoundChannel*, BL_AUDIO_STREAM_COUNT> channels = {};

		// The device ID for outgoing audio.
		SDL_AudioDeviceID deviceOut = 0;

	  private:
		[[maybe_unused]]
		char _padding[4];
	};

	// Stores the renderer's state.
	RenderState renderState;

	// Stores the audio mixer's state.
	AudioState audioState;

	// A pointer to the currently active scene. Must not be null, and should be set before the first
	// physics step is processed.
	std::unique_ptr<Scene> currentScene = nullptr;

	// A pointer to the window handle.
	SDL_Window* window;

	// The timestamp of the last render step. This is set at the end of the render step.
	uint64_t lastDraw = SDL_GetTicksNS();

	// The timestamp of the last physics step. This is set at the end of the physics step.
	uint64_t lastStep = SDL_GetTicksNS();

  public:
	// Changes the currently active scene.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult changeScene(Scene* newScene);

	// The callback that is fired on app initialisation.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult init(int argcount, char* args[]);

	// The callback that is fired on the physics step.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult step();

	// The callback that is fired on the render step.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult draw(uint64_t dt);

	// The callback that is fired whenever an input event is fired.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult processInput(const InputEvent& event);
};

// The global app state.
extern App* app;

}  // namespace Bloodlust
