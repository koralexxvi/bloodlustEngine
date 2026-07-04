#include "../include/app.hpp"

#include "../include/audio.hpp"
#include "../include/datatype/inputEvent.hpp"
#include "../include/logging.hpp"
#include "../include/render/graphics.hpp"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <format>
#include <unordered_map>

using Bloodlust::App;
std::unique_ptr<App> Bloodlust::app;

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weverything"
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_MALLOC(size) SDL_malloc(size)
#define STBI_REALLOC(ptr, size) SDL_realloc(ptr, size)
#define STBI_FREE(ptr) SDL_free(ptr)

#include <stb/stb_image.h>
#ifdef __clang__
	#pragma clang diagnostic pop
#endif

App::App(const EngineConfig& config, int, char*[]) noexcept : config(config) {
	SDL_SetAppMetadata(config.appName.c_str(), config.appVersion.c_str(), config.appID.c_str());
	window = std::make_unique<Window>(config.windowTitle, config.windowWidth, config.windowHeight);
}

SDL_AppResult App::changeScene(Scene* scene) noexcept {
	if (currentScene != nullptr) {
		SDL_AppResult exitResult = currentScene->onExit();
		if (exitResult != SDL_APP_CONTINUE) {
			return exitResult;
		}
	}
	currentScene = std::unique_ptr<Scene>(scene);
	return scene->onEnter();
}

void App::addGamepad(SDL_JoystickID joyID) noexcept {
	if (joyID == 0) {
		return;
	}
	SDL_Gamepad* gamepad = SDL_OpenGamepad(joyID);
	if (gamepad) {
		app->gamepads.insert({joyID, gamepad});
	} else {
		log(SDL_LOG_PRIORITY_ERROR, "Failed to open gamepad ({})", getError());
	}
}

void App::removeGamepad(SDL_JoystickID joyID) noexcept {
	if (joyID == 0 || !app->gamepads.contains(joyID)) {
		return;
	}
	SDL_CloseGamepad(app->gamepads[joyID]);
	app->gamepads.erase(joyID);
}

App::~App() noexcept {
	if (currentScene != nullptr) {
		static_cast<void>(currentScene->onExit());
	}
	currentScene = nullptr;
}

SDL_AppResult Bloodlust::onSDLAppInit(int argc, char* argv[]) noexcept {
#ifndef BL_PRODUCTION
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
#endif
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");
	log(SDL_LOG_PRIORITY_TRACE, "initialisation started");
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
		reportFailure(std::format("Internal error: Could not init SDL ({})", getError()));
	}
	if (!TTF_Init()) {
		reportFailure(std::format("Internal error: Could not init SDL_ttf ({})", getError()));
	}

	if (!initGraphics()) {
		reportFailure(
			std::format("Internal error: Could not init graphics subsystem ({})", getError())
		);
	}
	if (!initAudio()) {
		log(SDL_LOG_PRIORITY_ERROR,
			"Could not initialise the audio subsystem; audio will be unavailable ({})",
			getError());
	}
	if (!SDL_Init(SDL_INIT_GAMEPAD)) {
		log(SDL_LOG_PRIORITY_ERROR,
			"Could not initialise the gamepad subsystem; controllers will be unavailable ({})",
			getError());
	}

	SDL_srand(0);
	log(SDL_LOG_PRIORITY_TRACE, "engine initialisation finished");

	switch (app->init(argc, argv)) {
		case SDL_APP_CONTINUE: return SDL_APP_CONTINUE;
		case SDL_APP_SUCCESS: return SDL_APP_SUCCESS;
		case SDL_APP_FAILURE: reportFailure(getError());
		default: reportFailure("Invalid app result returned from App::init");
	}
}

SDL_AppResult Bloodlust::onSDLAppEvent(SDL_Event* event) noexcept {
	switch (event->type) {
		case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;

		case SDL_EVENT_WINDOW_RESIZED: {
			if (!onResize(
					static_cast<std::uint32_t>(event->window.data1),
					static_cast<std::uint32_t>(event->window.data2)
				))
			{
				reportFailure(std::format("Failed to resize window: {}", getError()));
			}
			break;
		}

		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN: return app->processInput(event->key);

		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN: return app->processInput(event->button);

		case SDL_EVENT_MOUSE_MOTION: return app->processInput(event->motion);

		case SDL_EVENT_MOUSE_WHEEL: return app->processInput(event->wheel);

		case SDL_EVENT_GAMEPAD_ADDED: app->addGamepad(event->gdevice.which); break;

		case SDL_EVENT_GAMEPAD_REMOVED: app->removeGamepad(event->gdevice.which); break;

		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN: return app->processInput(event->gbutton);

		case SDL_EVENT_GAMEPAD_AXIS_MOTION: return app->processInput(event->gaxis);

		default: break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult Bloodlust::onSDLAppIterate() noexcept {
	std::uint64_t now = SDL_GetTicksNS();
	SDL_AppResult result = SDL_APP_CONTINUE;
	std::uint32_t steps = 0;
	while (app->lastStep < now - app->getConfig().appTimestep) {
		steps++;
		if (steps > app->getConfig().appMaxCatchupSteps) {
			break;
		}
		result = app->step();
		app->lastStep += app->getConfig().appTimestep;
		if (result != SDL_APP_CONTINUE) {
			reportFailure(std::format("Failed to run update step: {}", getError()));
		}
	}

	if (!processAudio()) {
		reportFailure(std::format("Failed to process audio: {}", getError()));
	}

	clearAll();
	result = app->draw(now - app->lastDraw);
	if (result != SDL_APP_CONTINUE) {
		reportFailure(std::format("Failed to run render step: {}", getError()));
	}
	app->lastDraw = now;
	if (!runRenderPass()) {
		reportFailure(std::format("Failed to run render pass: {}", getError()));
	}
	return SDL_APP_CONTINUE;
}

void Bloodlust::quit(int exitCode) noexcept {
	closeAudio();
	closeGraphics();
	app = nullptr;
	TTF_Quit();
	SDL_QuitSubSystem(
		SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_VIDEO
	);
	SDL_Quit();
	std::exit(exitCode);
}
