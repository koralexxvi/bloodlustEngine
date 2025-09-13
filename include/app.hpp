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
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <unordered_map>
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
  private:
	// The structure that dictates the configuration of the running app.
	struct EngineConfig {
		// The name of the app.
		const char* appName = "BloodlustApp";

		// The version of the app.
		const char* appVersion = "0.0.0";

		// The app's metadata ID.
		const char* appID = "com.example.bloodlustapp";

		// The text to be shown on the window's titlebar.
		const char* windowTitle = appName;

		// The initial width of the window.
		const std::uint32_t windowWidth = 960;

		// The initial height of the window.
		const std::uint32_t windowHeight = 540;

		// The time between calls to App::step(), in nanoseconds. Defaults to 60fps.
		std::uint64_t appTimestep = 1'000'000'000LL / 60LL;

		// How many steps the app is allowed to process at a time to keep up with realtime.
		// Must be greater than 0.
		std::uint32_t appMaxCatchupSteps = 4;

		[[maybe_unused]]
		char _padding[4] = {};
	};

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

	const EngineConfig config;

	std::unordered_map<SDL_JoystickID, SDL_Gamepad*> gamepads = {};

  public:
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

	App(const EngineConfig& config, int argc, char* argv[]);

	// Returns a const reference to the app's configuration.
	constexpr const EngineConfig& getConfig() {
		return config;
	}

	// Changes the currently active scene.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	SDL_AppResult changeScene(Scene* newScene);

	void addGamepad(SDL_JoystickID joyID);

	void removeGamepad(SDL_JoystickID joyID);

	// The callback that is fired on app initialisation.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult init(int argc, char* argv[]) = 0;

	// The callback that is fired on the physics step.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult step() = 0;

	// The callback that is fired on the render step.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult draw(std::uint64_t dt) = 0;

	// The callback that is fired whenever an input event is fired.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult processInput(const InputEvent& event) = 0;

	virtual ~App();
};

// The global app state.
extern std::unique_ptr<App> app;

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
SDL_AppResult onSDLAppInit(int argc, char* argv[]);

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
SDL_AppResult onSDLAppEvent(SDL_Event* event);

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
SDL_AppResult onSDLAppIterate();

// Exits the program, with an optional exit code (defaults to 0).
[[noreturn]]
void quit(int exitCode = 0);

}  // namespace Bloodlust
