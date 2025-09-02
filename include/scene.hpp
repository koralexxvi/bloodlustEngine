#pragma once

#include "datatype/inputEvent.hpp"
#include "util.hpp"

#include <SDL3/SDL_init.h>
#include <cstdint>

namespace Bloodlust {

struct Scene {
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult onEnter();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult step() = 0;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult draw(std::uint64_t dt) = 0;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult onInput(const InputEvent& input) = 0;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual SDL_AppResult onExit();

	virtual ~Scene() = default;
};

}  // namespace Bloodlust
