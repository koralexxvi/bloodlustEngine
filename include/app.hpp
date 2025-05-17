#ifndef BL_APP_H
#define BL_APP_H

#include "datatype/inputEvent.hpp"
#include "scene.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>

#ifdef APP_TIMESTEP
static_assert(APP_TIMESTEP > 0, "config error: APP_TIMESTEP must be greater than 0");
#else
	#define APP_TIMESTEP (1'000'000'000LL / 60LL)
#endif

#ifdef APP_MAX_CATCHUP_STEPS
static_assert(
	APP_MAX_CATCHUP_STEPS > 0,
	"config error: APP_MAX_CATCHUP_STEPS must be greater than 0"
);
#else
	#define APP_MAX_CATCHUP_STEPS 4
#endif

namespace Bloodlust {

enum class AppResult {
	CONTINUE,
	SUCCESS,
	FAILURE
};

struct App {
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_TextEngine* textEngine;

	Scene* currentScene;

	uint64_t lastDraw = SDL_GetTicksNS();
	uint64_t lastStep = SDL_GetTicksNS();

	virtual AppResult init(int argcount, char* args[]) = 0;
	virtual AppResult step() = 0;
	virtual AppResult draw(uint64_t dt) = 0;

	virtual bool processInput(const DataTypes::InputEvent& event) = 0;

	virtual ~App() = default;
};

extern App* app;

}  // namespace Bloodlust

#endif
