#ifndef BL_SCENE_H
#define BL_SCENE_H

#include "datatype/inputEvent.hpp"

#include <cstdint>

namespace Bloodlust {

struct Scene {
	virtual bool onEnter() = 0;

	virtual bool step() = 0;
	virtual bool draw(uint64_t dt) = 0;
	virtual bool onInput(const InputEvent& input) = 0;

	virtual bool onExit() = 0;

	virtual ~Scene() = default;
};

}  // namespace Bloodlust

#endif
