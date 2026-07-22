#include <SDL3/SDL_video.h>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>
#include <bloodlustEngine/render/window.hpp>

using Bloodlust::Window;

Window::Window(std::string_view title, std::uint32_t width, std::uint32_t height)
	: width(width),
	  height(height) {
	windowHandle = SDL_CreateWindow(
		std::string(title).c_str(),
		static_cast<std::int32_t>(width),
		static_cast<std::int32_t>(height),
		0
	);
	if (windowHandle == nullptr) {
		throw InternalError(std::format("Could not create window: {}", getError()));
	}
}

Window::~Window() noexcept {
	SDL_DestroyWindow(windowHandle);
}

bool Window::setWindowed() noexcept {
	return SDL_SetWindowFullscreen(windowHandle, false);
}

bool Window::setBorderlessFullscreen(SDL_DisplayID display) noexcept {
	SDL_Rect bounds;
	return SDL_GetDisplayUsableBounds(display, &bounds)
		&& SDL_SetWindowPosition(windowHandle, bounds.x, bounds.y)
		&& SDL_SetWindowFullscreenMode(windowHandle, nullptr);
}

bool Window::setExclusiveFullscreen(SDL_DisplayID display) noexcept {
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
	if (mode == nullptr) {
		return false;
	}
	return SDL_SetWindowFullscreenMode(windowHandle, mode);
}
