#include "../../include/render/window.hpp"

#include "../../include/config.hpp"

#include <SDL3/SDL_video.h>

using Bloodlust::Window;

Window::Window(std::uint32_t width, std::uint32_t height, bool& success)
	: width(width),
	  height(height) {
	windowHandle = SDL_CreateWindow(
		config.windowTitle,
		static_cast<std::int32_t>(width),
		static_cast<std::int32_t>(height),
		0
	);
	success = windowHandle != nullptr;
}

SDL_Window* Window::getHandle() const {
	return windowHandle;
}

std::uint32_t Window::getWidth() const {
	return width;
}

std::uint32_t Window::getHeight() const {
	return height;
}

Bloodlust::Vector2 Window::getSize() const {
	return {static_cast<float>(width), static_cast<float>(height)};
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
