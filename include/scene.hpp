#pragma once

#include "datatype/inputEvent.hpp"

#include <cstdint>

namespace Bloodlust {

struct Scene {
	virtual bool onEnter();

	virtual bool step() = 0;
	virtual bool draw(uint64_t dt) = 0;
	virtual bool onInput(const InputEvent& input) = 0;

	virtual bool onExit();

	virtual ~Scene() = default;
};

}  // namespace Bloodlust
