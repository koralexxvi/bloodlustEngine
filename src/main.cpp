#include "../include/main.hpp"

#include "../include/app.hpp"
#include "../include/config.hpp"
#include "../include/datatype/inputEvent.hpp"
#include "../include/logging.hpp"
#include "../include/render/graphics.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_video.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <format>
#include <unordered_map>

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weverything"
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_MALLOC(size) SDL_malloc(size)
#define STBI_REALLOC(ptr, size) SDL_realloc(ptr, size)
#define STBI_FREE SDL_free

#include "../extern/stb_image.h"  // IWYU pragma: keep
#ifdef __clang__
	#pragma clang diagnostic pop
#endif

namespace {

std::unordered_map<SDL_JoystickID, SDL_Gamepad*> gamepads = {};

[[nodiscard("should be used as a return value")]]
SDL_AppResult reportFailure(const std::string&& errMsg, bool noWindow = false) {
	Bloodlust::log("A fatal error has occurred!");
	Bloodlust::log("Reason: %s", errMsg.c_str());
	if (!noWindow) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Fatal Error",
			std::format("A fatal error has occurred!\nReason: {}", errMsg).c_str(),
			nullptr
		);
	}
	return SDL_APP_FAILURE;
}

}  // namespace

Bloodlust::App* Bloodlust::app;
Bloodlust::EngineConfig* Bloodlust::config;

SDL_AppResult Bloodlust::init(int argcount, char* args[]) {
#ifndef BL_PRODUCTION
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
#endif
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");
	log(SDL_LOG_PRIORITY_TRACE, "initialisation started");

	if (app == nullptr) {
		return reportFailure("No app found");
	}
	if (config == nullptr) {
		config = new EngineConfig{};
	}
	SDL_SetAppMetadata(config->appName, config->appVersion, config->appID);
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
		return reportFailure(std::format("Internal error: Could not init SDL ({})", getError()));
	}

	if (Mix_Init(MIX_INIT_OGG) == 0) {
		return reportFailure(
			std::format("Internal error: Could not init SDL_mixer ({})", getError())
		);
	}
	if (!TTF_Init()) {
		return reportFailure(
			std::format("Internal error: Could not init SDL_ttf ({})", getError())
		);
	}

	SDL_Window* window = SDL_CreateWindow(
		config->windowTitle,
		int(config->windowWidth),
		int(config->windowHeight),
		0
	);
	if (window == nullptr) {
		return reportFailure(
			std::format("Internal error: Could not create window ({})", getError())
		);
	}
	app->window = window;

	if (!initGraphics()) {
		return reportFailure(
			std::format("Internal error: Could not init graphics subsystem ({})", getError())
		);
	}
	if (!SDL_Init(SDL_INIT_AUDIO)) {
		log(SDL_LOG_PRIORITY_ERROR,
			"Could not initialise the audio subsystem; audio will be unavailable (%s)",
			getError());
	}
	if (!SDL_Init(SDL_INIT_GAMEPAD)) {
		log(SDL_LOG_PRIORITY_ERROR,
			"Could not initialise the gamepad subsystem; controllers will be unavailable (%s)",
			getError());
	}

	SDL_srand(0);
	log(SDL_LOG_PRIORITY_TRACE, "engine initialisation finished");

	switch (app->init(argcount, args)) {
		case AppResult::CONTINUE: return SDL_APP_CONTINUE;
		case AppResult::SUCCESS: return SDL_APP_SUCCESS;
		case AppResult::FAILURE: return reportFailure(getError());
		default: return reportFailure("Invalid app result returned from App::init");
	}
}

SDL_AppResult Bloodlust::processEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;

		case SDL_EVENT_WINDOW_RESIZED: {
			if (!onResize(uint32_t(event->window.data1), uint32_t(event->window.data2))) {
				return reportFailure(std::format("Failed to resize window: {}", getError()));
			}
			break;
		}

		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN: app->processInput(InputEvent(event->key)); break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN: app->processInput(InputEvent(event->button)); break;

		case SDL_EVENT_MOUSE_MOTION: app->processInput(InputEvent(event->motion)); break;

		case SDL_EVENT_MOUSE_WHEEL: app->processInput(InputEvent(event->wheel)); break;

		case SDL_EVENT_GAMEPAD_ADDED: {
			SDL_JoystickID id = event->gdevice.which;
			if (id == 0) {
				break;
			}
			SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
			if (gamepad) {
				gamepads.insert({id, gamepad});
			} else {
				log(SDL_LOG_PRIORITY_ERROR, "Failed to open gamepad (%s)", getError());
			}
			break;
		}
		case SDL_EVENT_GAMEPAD_REMOVED: {
			SDL_JoystickID id = event->gdevice.which;
			if (id == 0 || !gamepads.contains(id)) {
				break;
			}
			SDL_CloseGamepad(gamepads[id]);
			gamepads.erase(id);
			break;
		}

		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN: app->processInput(InputEvent(event->gbutton)); break;

		case SDL_EVENT_GAMEPAD_AXIS_MOTION: app->processInput(InputEvent(event->gaxis)); break;

		default: break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult Bloodlust::iterate() {
	uint64_t now = SDL_GetTicksNS();
	AppResult result = AppResult::CONTINUE;
	unsigned steps = 0;
	while (app->lastStep < now) {
		steps++;
		if (steps > config->appMaxCatchupSteps) {
			break;
		}
		result = app->step();
		app->lastStep += config->appTimestep;
		if (result != AppResult::CONTINUE) {
			return reportFailure(std::format("Failed to run update step: {}", getError()));
		}
	}

	clearAll();
	result = app->draw(now - app->lastDraw);
	if (result != AppResult::CONTINUE) {
		return reportFailure(std::format("Failed to run render step: {}", getError()));
	}
	app->lastDraw = now;
	if (!runRenderPass()) {
		return reportFailure(std::format("Failed to run render pass: {}", getError()));
	}
	return SDL_APP_CONTINUE;
}

void Bloodlust::quit(SDL_AppResult result) {
	delete app->renderState.rectFillShader;
	delete app->renderState.textureShader;
	delete app->renderState.textShader;
	if (app->currentScene != nullptr) {
		app->currentScene->onExit();
	}
	app->currentScene = nullptr;
	TTF_DestroyGPUTextEngine(app->renderState.textEngine);
	SDL_ReleaseGPUTexture(app->renderState.device, app->renderState.depthTexture);
	SDL_DestroyGPUDevice(app->renderState.device);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
}
