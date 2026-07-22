#pragma once

#include "util.hpp"

#include <SDL3/SDL_init.h>
#include <cstdint>

namespace Bloodlust {

struct InputEvent;

struct Scene {
	virtual ~Scene() noexcept = default;

	// A callback that is fired when the scene is entered, after the last scene's onEnter callback
	// is fired.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult onEnter() noexcept;

	// A callback that is fired at the start of each physics step.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult step() noexcept;

	// A callback that is fired at the start of each render step.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult draw(std::uint64_t dt) noexcept;

	// A callback that is fired on each input event.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult onInput(const InputEvent& input) noexcept;

	// A callback that is fired when the scene is exited, but before the new scene's onEnter
	// callback is fired.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult onExit() noexcept;
};

}  // namespace Bloodlust
