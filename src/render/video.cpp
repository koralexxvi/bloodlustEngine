#include "../../include/render/video.hpp"

#include "../../include/app.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_video.h>

Bloodlust::Vector2 Bloodlust::getResolution() {
	int x, y;
	if (!SDL_GetWindowSize(app->window, &x, &y)) {
		return {};
	}
	return {float(x), float(y)};
}

bool Bloodlust::setWindowed() {
	return SDL_SetWindowFullscreen(app->window, false);
}

bool Bloodlust::setBorderlessFullscreen(SDL_DisplayID display) {
	SDL_Rect bounds;
	return SDL_GetDisplayUsableBounds(display, &bounds)
		&& SDL_SetWindowPosition(app->window, bounds.x, bounds.y)
		&& SDL_SetWindowFullscreenMode(app->window, nullptr);
}

bool Bloodlust::setExclusiveFullscreen(SDL_DisplayID display) {
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
	if (mode == nullptr) {
		return false;
	}
	return SDL_SetWindowFullscreenMode(app->window, mode);
}
