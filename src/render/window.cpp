#include "../../include/render/window.hpp"

#include "../../include/app.hpp"

#include <SDL3/SDL_video.h>

using Bloodlust::Window;

Window::Window(std::uint32_t width, std::uint32_t height, bool& success)
	: width(width),
	  height(height) {
	windowHandle = SDL_CreateWindow(
		app->getConfig().windowTitle,
		static_cast<std::int32_t>(width),
		static_cast<std::int32_t>(height),
		0
	);
	success = windowHandle != nullptr;
}

bool Window::setWindowed() {
	return SDL_SetWindowFullscreen(windowHandle, false);
}

bool Window::setBorderlessFullscreen(SDL_DisplayID display) {
	SDL_Rect bounds;
	return SDL_GetDisplayUsableBounds(display, &bounds)
		&& SDL_SetWindowPosition(windowHandle, bounds.x, bounds.y)
		&& SDL_SetWindowFullscreenMode(windowHandle, nullptr);
}

bool Window::setExclusiveFullscreen(SDL_DisplayID display) {
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
	if (mode == nullptr) {
		return false;
	}
	return SDL_SetWindowFullscreenMode(windowHandle, mode);
}

Window::~Window() {
	SDL_DestroyWindow(windowHandle);
}
