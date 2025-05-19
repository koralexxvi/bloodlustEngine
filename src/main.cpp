#include "../include/main.hpp"

#include "../include/app.hpp"
#include "../include/config.hpp"
#include "../include/datatype/inputEvent.hpp"
#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace Bloodlust;
using Logging::LogPriority;

App* Bloodlust::app;
EngineConfig* Bloodlust::config;

static void reportInitFailure(const std::string& errMsg) {
	Logging::log(errMsg, LogPriority::CRITICAL);
	Logging::log(SDL_GetError());
}

SDL_AppResult Bloodlust::init(int argcount, char* args[]) {
	SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
	Logging::log("initialisation started", LogPriority::TRACE);

	if (config == nullptr) {
		config = new EngineConfig{};
	}
	SDL_SetAppMetadata(config->appName, config->appVersion, config->appID);
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");

	if (app == nullptr) {
		Logging::log("No app found", LogPriority::CRITICAL);
		return SDL_APP_FAILURE;
	}

	if (Mix_Init(MIX_INIT_OGG) == 0) {
		reportInitFailure("Internal error: Could not init SDL_mixer");
		return SDL_APP_FAILURE;
	}
	if (!TTF_Init()) {
		reportInitFailure("Internal error: Could not init SDL_ttf");
		return SDL_APP_FAILURE;
	}

	SDL_Window* window =
		SDL_CreateWindow(config->windowTitle, config->windowWidth, config->windowHeight, 0);
	if (window == nullptr) {
		reportInitFailure("Internal error: Could not create window");
	}
	app->window = window;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	if (renderer == nullptr) {
		reportInitFailure("Internal error: Could not create the SDL renderer");
		return SDL_APP_FAILURE;
	}
	app->renderer = renderer;
	if (!SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE)) {
		SDL_SetRenderVSync(renderer, 1);
	}

	TTF_TextEngine* textEngine = TTF_CreateRendererTextEngine(renderer);
	if (textEngine == nullptr) {
		reportInitFailure("Internal error: Could not create a text engine");
		return SDL_APP_FAILURE;
	}
	app->textEngine = textEngine;

	Logging::log("engine initialisation finished", LogPriority::TRACE);

	switch (app->init(argcount, args)) {
		case AppResult::CONTINUE: return SDL_APP_CONTINUE;
		case AppResult::SUCCESS: return SDL_APP_SUCCESS;
		case AppResult::FAILURE: reportInitFailure(Logging::getError()); return SDL_APP_FAILURE;
		default: {
			reportInitFailure("Invalid app result returned from App::init");
			return SDL_APP_FAILURE;
		}
	}
}

SDL_AppResult Bloodlust::processEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;

		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN: app->processInput(InputEvent(event->key)); break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN: app->processInput(InputEvent(event->button)); break;

		case SDL_EVENT_MOUSE_MOTION: app->processInput(InputEvent(event->motion)); break;

		case SDL_EVENT_MOUSE_WHEEL: app->processInput(InputEvent(event->wheel)); break;

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

	int steps = 0;
	while (app->lastStep < now) {
		steps++;
		if (steps > APP_MAX_CATCHUP_STEPS) {
			break;
		}
		result = app->step();
		app->lastStep += APP_TIMESTEP;
		if (result != AppResult::CONTINUE) {
			goto exit;
		}
	}

	SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
	SDL_RenderClear(app->renderer);
	result = app->draw(now - app->lastDraw);
	if (result != AppResult::CONTINUE) {
		goto exit;
	}
	app->lastDraw = now;
	SDL_RenderPresent(app->renderer);
	return SDL_APP_CONTINUE;

exit:
	switch (result) {
		case AppResult::SUCCESS: return SDL_APP_SUCCESS;
		case AppResult::FAILURE: {
			Logging::log(Logging::getError(), LogPriority::CRITICAL);
			return SDL_APP_FAILURE;
		}
		default: {
			Logging::log("Invalid app result returned", LogPriority::CRITICAL);
			return SDL_APP_FAILURE;
		}
	}
}

void Bloodlust::quit(SDL_AppResult result) {
	Logging::log("exiting", LogPriority::TRACE);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
}
