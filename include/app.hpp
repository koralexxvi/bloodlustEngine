#pragma once

#include "datatype/inputEvent.hpp"
#include "datatype/soundChannel.hpp"
#include "render/lineShader.hpp"
#include "render/rectFillShader.hpp"
#include "render/textShader.hpp"
#include "render/textureShader.hpp"
#include "render/window.hpp"
#include "scene.hpp"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <vector>

#ifdef BL_LAYER_COUNT
	#if BL_LAYER_COUNT < 1
		#error BL_LAYER_COUNT must be greater than 0
	#endif
#else
	#define BL_LAYER_COUNT (32)
#endif

#ifdef BL_UI_LAYER
	#if BL_UI_LAYER < 0 || BL_UI_LAYER >= BL_LAYER_COUNT
		#error invalid value provided for BL_UI_LAYER
	#endif
#elif BL_LAYER_COUNT == 1
	#define BL_UI_LAYER (0)
#else
	#define BL_UI_LAYER (BL_LAYER_COUNT - 2)
#endif

#ifdef BL_SOUND_CHANNEL_COUNT
	#if BL_SOUND_CHANNEL_COUNT < 1
		#error BL_SOUND_CHANNEL_COUNT must be greater than 0
	#endif
#else
	#define BL_SOUND_CHANNEL_COUNT (8)
#endif

namespace Bloodlust {

// The global state for the running app.
struct App {
	// A struct that contains the renderer's state.
	struct RenderState {
	  private:
		struct InternalShaders {
			// A pointer to the internal line shader.
			std::unique_ptr<LineShader> line = {};

			// A pointer to the internal rect fill shader.
			std::unique_ptr<RectFillShader> rectFill = {};

			// A pointer to the internal text shader.
			std::unique_ptr<TextShader> text = {};

			// A pointer to the internal texture shader.
			std::unique_ptr<TextureShader> texture = {};
		};

	  public:
		// A pointer to the GPU device handle.
		SDL_GPUDevice* device;

		// A pointer to the internal text engine used for drawing glyphs.
		TTF_TextEngine* textEngine;

		// A pointer to the depth texture.
		SDL_GPUTexture* depthTexture;

		// An array of the pointers to internal shaders that handle drawing for each layer.
		std::array<InternalShaders, BL_LAYER_COUNT> internalShaders = {};

		// An array of clip rects to be used by the internal shaders.
		std::vector<SDL_Rect> clipRects = {};
	};

	// A struct that contains the audio mixer's state.
	struct AudioState {
		// An array of the available sound channels. The amount of channels is defined by
		// BL_SOUND_CHANNEL_COUNT and defaults to 8.
		std::array<std::unique_ptr<SoundChannel>, BL_SOUND_CHANNEL_COUNT> channels = {};

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
	std::unique_ptr<Window> window = nullptr;

	// The timestamp of the last render step. This is set at the end of the render step.
	std::uint64_t lastDraw = SDL_GetTicksNS();

	// The timestamp of the last physics step. This is set at the end of the physics step.
	std::uint64_t lastStep = SDL_GetTicksNS();

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
	SDL_AppResult draw(std::uint64_t dt);

	// The callback that is fired whenever an input event is fired.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult processInput(const InputEvent& event);
};

// The global app state.
extern App* app;

}  // namespace Bloodlust
