#pragma once

#include "../datatype/point.hpp"
#include "../util.hpp"

#include <SDL3/SDL_video.h>
#include <cstdint>

namespace Bloodlust {

class Window {
  private:
	SDL_Window* windowHandle;

	std::uint32_t width;
	std::uint32_t height;

  public:
	Window(std::string_view title, std::uint32_t width, std::uint32_t height) noexcept;

	// Returns the internal window handle.
	constexpr SDL_Window* getHandle() const noexcept {
		return windowHandle;
	}

	// Returns the width of the window.
	constexpr std::uint32_t getWidth() const noexcept {
		return width;
	}

	// Returns the height of the window.
	constexpr std::uint32_t getHeight() const noexcept {
		return height;
	}

	// Returns the dimensions of the window.
	constexpr Point<std::uint32_t> getSize() const noexcept {
		return {width, height};
	}

	// Sets the fullscreen mode to windowed.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool setWindowed() noexcept;

	// Sets the fullscreen mode to borderless fullscreen on the given display.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool setBorderlessFullscreen(SDL_DisplayID display) noexcept;

	// Sets the fullscreen mode to exclusive fullscreen on the given display.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool setExclusiveFullscreen(SDL_DisplayID display) noexcept;

	~Window() noexcept;
};

}  // namespace Bloodlust
