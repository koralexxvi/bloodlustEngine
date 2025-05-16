#include "../include/video.hpp"

#include "../include/app.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

using namespace Bloodlust;

DataTypes::Vector2 Video::getResolution() {
	int x, y;
	if (!SDL_GetCurrentRenderOutputSize(app->renderer.get(), &x, &y)) {
		return {};
	}
	return {float(x), float(y)};
}

bool Video::setWindowed() {
	return SDL_SetWindowFullscreen(app->window.get(), false);
}

bool Video::setBorderlessFullscreen(SDL_DisplayID display) {
	SDL_Rect bounds;
	return SDL_GetDisplayUsableBounds(display, &bounds)
		&& SDL_SetWindowPosition(app->window.get(), bounds.x, bounds.y)
		&& SDL_SetWindowFullscreenMode(app->window.get(), nullptr);
}

bool Video::setExclusiveFullscreen(SDL_DisplayID display) {
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
	if (mode == nullptr) {
		return false;
	}
	return SDL_SetWindowFullscreenMode(app->window.get(), mode);
}
