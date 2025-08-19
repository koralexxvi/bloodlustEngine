#include "../include/main.hpp"

#include "../include/app.hpp"
#include "../include/audio.hpp"
#include "../include/config.hpp"
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

// clang-format off
#define SDL_MAIN_USE_CALLBACKS (1)
#include <SDL3/SDL_main.h>
// clang-format on

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

SDL_AppResult SDL_AppInit(void**, int argcount, char* args[]) {
#ifndef BL_PRODUCTION
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
#endif
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");
	Bloodlust::log(SDL_LOG_PRIORITY_TRACE, "initialisation started");

	SDL_SetAppMetadata(
		Bloodlust::config.appName,
		Bloodlust::config.appVersion,
		Bloodlust::config.appID
	);
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
		return reportFailure(
			std::format("Internal error: Could not init SDL ({})", Bloodlust::getError())
		);
	}

	if (!TTF_Init()) {
		return reportFailure(
			std::format("Internal error: Could not init SDL_ttf ({})", Bloodlust::getError())
		);
	}

	Bloodlust::app = new Bloodlust::App();

	SDL_Window* window = SDL_CreateWindow(
		Bloodlust::config.windowTitle,
		int(Bloodlust::config.windowWidth),
		int(Bloodlust::config.windowHeight),
		0
	);
	if (window == nullptr) {
		return reportFailure(
			std::format("Internal error: Could not create window ({})", Bloodlust::getError())
		);
	}
	Bloodlust::app->window = window;

	if (!Bloodlust::initGraphics()) {
		return reportFailure(
			std::format(
				"Internal error: Could not init graphics subsystem ({})",
				Bloodlust::getError()
			)
		);
	}
	if (!Bloodlust::initAudio()) {
		Bloodlust::log(
			SDL_LOG_PRIORITY_ERROR,
			"Could not initialise the audio subsystem; audio will be unavailable (%s)",
			Bloodlust::getError()
		);
	}
	if (!SDL_Init(SDL_INIT_GAMEPAD)) {
		Bloodlust::log(
			SDL_LOG_PRIORITY_ERROR,
			"Could not initialise the gamepad subsystem; controllers will be unavailable (%s)",
			Bloodlust::getError()
		);
	}

	SDL_srand(0);
	Bloodlust::log(SDL_LOG_PRIORITY_TRACE, "engine initialisation finished");

	switch (Bloodlust::app->init(argcount, args)) {
		case SDL_APP_CONTINUE: return SDL_APP_CONTINUE;
		case SDL_APP_SUCCESS: return SDL_APP_SUCCESS;
		case SDL_APP_FAILURE: return reportFailure(Bloodlust::getError());
		default: return reportFailure("Invalid app result returned from App::init");
	}
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;

		case SDL_EVENT_WINDOW_RESIZED: {
			if (!Bloodlust::onResize(uint32_t(event->window.data1), uint32_t(event->window.data2)))
			{
				return reportFailure(
					std::format("Failed to resize window: {}", Bloodlust::getError())
				);
			}
			break;
		}

		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			return Bloodlust::app->processInput(Bloodlust::InputEvent(event->key));

		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			return Bloodlust::app->processInput(Bloodlust::InputEvent(event->button));

		case SDL_EVENT_MOUSE_MOTION:
			return Bloodlust::app->processInput(Bloodlust::InputEvent(event->motion));

		case SDL_EVENT_MOUSE_WHEEL:
			return Bloodlust::app->processInput(Bloodlust::InputEvent(event->wheel));

		case SDL_EVENT_GAMEPAD_ADDED: {
			SDL_JoystickID id = event->gdevice.which;
			if (id == 0) {
				break;
			}
			SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
			if (gamepad) {
				gamepads.insert({id, gamepad});
			} else {
				Bloodlust::log(
					SDL_LOG_PRIORITY_ERROR,
					"Failed to open gamepad (%s)",
					Bloodlust::getError()
				);
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
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			return Bloodlust::app->processInput(Bloodlust::InputEvent(event->gbutton));

		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			return Bloodlust::app->processInput(Bloodlust::InputEvent(event->gaxis));

		default: break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void*) {
	uint64_t now = SDL_GetTicksNS();
	SDL_AppResult result = SDL_APP_CONTINUE;
	unsigned steps = 0;
	while (Bloodlust::app->lastStep < now - Bloodlust::config.appTimestep) {
		steps++;
		if (steps > Bloodlust::config.appMaxCatchupSteps) {
			break;
		}
		result = Bloodlust::app->step();
		Bloodlust::app->lastStep += Bloodlust::config.appTimestep;
		if (result != SDL_APP_CONTINUE) {
			return reportFailure(
				std::format("Failed to run update step: {}", Bloodlust::getError())
			);
		}
	}

	if (!Bloodlust::processAudio()) {
		return reportFailure(std::format("Failed to process audio: {}", Bloodlust::getError()));
	}

	Bloodlust::clearAll();
	result = Bloodlust::app->draw(now - Bloodlust::app->lastDraw);
	if (result != SDL_APP_CONTINUE) {
		return reportFailure(std::format("Failed to run render step: {}", Bloodlust::getError()));
	}
	Bloodlust::app->lastDraw = now;
	if (!Bloodlust::runRenderPass()) {
		return reportFailure(std::format("Failed to run render pass: {}", Bloodlust::getError()));
	}
	return SDL_APP_CONTINUE;
}

void Bloodlust::quit(int exitCode) {
	if (app != nullptr) {
		if (app->currentScene != nullptr) {
			(void) app->currentScene->onExit();
		}
		app->currentScene = nullptr;
		closeAudio();
		closeGraphics();
	}
	TTF_Quit();
	SDL_QuitSubSystem(
		SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_VIDEO
	);
	SDL_Quit();
	std::exit(exitCode);
}

void SDL_AppQuit(void*, SDL_AppResult result) {
	Bloodlust::quit(result == SDL_APP_FAILURE);
}
