#pragma once

#include "datatype/inputEvent.hpp"

#include <cstdint>

namespace Bloodlust {

enum class AppResult;

struct Scene {
	virtual AppResult onEnter();

	virtual AppResult step() = 0;
	virtual AppResult draw(uint64_t dt) = 0;
	virtual AppResult onInput(const InputEvent& input) = 0;

	virtual AppResult onExit();

	virtual ~Scene() = default;
};

}  // namespace Bloodlust
