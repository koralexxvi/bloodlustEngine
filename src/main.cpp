#include "../include/main.hpp"

#include "../include/app.hpp"
#include "../include/config.hpp"
#include "../include/datatype/inputEvent.hpp"
#include "../include/graphics.hpp"
#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <unordered_map>

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weverything"
#endif
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#include "../extern/stb_image.h"  // IWYU pragma: keep
#ifdef __clang__
	#pragma clang diagnostic pop
#endif

using Bloodlust::LogPriority;

Bloodlust::App* Bloodlust::app;
Bloodlust::EngineConfig* Bloodlust::config;

namespace {

std::unordered_map<SDL_JoystickID, SDL_Gamepad*> gamepads = {};

void reportInitError(const std::string& errMsg, LogPriority priority = LogPriority::CRITICAL) {
	Bloodlust::log(errMsg, priority);
	// Bloodlust::Logging::log(SDL_GetError());
	SDL_Log("%s", SDL_GetError());
}

}  // namespace

SDL_AppResult Bloodlust::init(int argcount, char* args[]) {
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
	log("initialisation started", LogPriority::TRACE);

	if (config == nullptr) {
		config = new EngineConfig{};
	}
	if (app == nullptr) {
		log("No app found", LogPriority::CRITICAL);
		return SDL_APP_FAILURE;
	}
	SDL_SetAppMetadata(config->appName, config->appVersion, config->appID);
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
		reportInitError("Internal error: Could not init SDL");
		return SDL_APP_FAILURE;
	}

	if (Mix_Init(MIX_INIT_OGG) == 0) {
		reportInitError("Internal error: Could not init SDL_mixer");
		return SDL_APP_FAILURE;
	}
	if (!TTF_Init()) {
		reportInitError("Internal error: Could not init SDL_ttf");
		return SDL_APP_FAILURE;
	}

	SDL_Window* window = SDL_CreateWindow(
		config->windowTitle,
		int(config->windowWidth),
		int(config->windowHeight),
		0
	);
	if (window == nullptr) {
		reportInitError("Internal error: Could not create window");
	}
	app->window = window;

	if (!initGraphics()) {
		reportInitError("Internal error: Could not init graphics subsystem");
		return SDL_APP_FAILURE;
	}
	if (!SDL_Init(SDL_INIT_AUDIO)) {
		reportInitError(
			"Could not initialise the audio subsystem; audio will be unavailable",
			LogPriority::ERROR
		);
	}
	if (!SDL_Init(SDL_INIT_GAMEPAD)) {
		reportInitError(
			"Could not initialise the gamepad subsystem; controllers will be unavailable",
			LogPriority::ERROR
		);
	}

	log("engine initialisation finished", LogPriority::TRACE);

	switch (app->init(argcount, args)) {
		case AppResult::CONTINUE: return SDL_APP_CONTINUE;
		case AppResult::SUCCESS: return SDL_APP_SUCCESS;
		case AppResult::FAILURE: reportInitError(getError()); return SDL_APP_FAILURE;
		default: {
			reportInitError("Invalid app result returned from App::init");
			return SDL_APP_FAILURE;
		}
	}
}

SDL_AppResult Bloodlust::processEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;

		case SDL_EVENT_WINDOW_RESIZED: {
			if (!onResize(uint32_t(event->window.data1), uint32_t(event->window.data2))) {
				return SDL_APP_FAILURE;
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
				log("Failed to open gamepad", LogPriority::ERROR);
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
			goto exit;
		}
	}

	clearAll();
	result = app->draw(now - app->lastDraw);
	if (result != AppResult::CONTINUE) {
		goto exit;
	}
	app->lastDraw = now;
	if (!runRenderPass()) {
		result = AppResult::FAILURE;
		goto exit;
	}
	return SDL_APP_CONTINUE;

exit:
	switch (result) {
		case AppResult::SUCCESS: return SDL_APP_SUCCESS;
		case AppResult::FAILURE: {
			log(getError(), LogPriority::CRITICAL);
			return SDL_APP_FAILURE;
		}
		default: {
			log("Invalid app result returned", LogPriority::CRITICAL);
			return SDL_APP_FAILURE;
		}
	}
}

void Bloodlust::quit(SDL_AppResult result) {
	log("exiting", LogPriority::TRACE);
	delete app->renderState.rectFillShader;
	delete app->renderState.textureShader;
	delete app->renderState.textShader;
	TTF_DestroyGPUTextEngine(app->renderState.textEngine);
	SDL_ReleaseGPUTexture(app->renderState.device, app->renderState.depthTexture);
	SDL_DestroyGPUDevice(app->renderState.device);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
}
