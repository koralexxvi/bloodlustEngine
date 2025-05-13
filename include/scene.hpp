#ifndef BL_SCENE_H
#define BL_SCENE_H

#include <cstdint>

namespace Bloodlust {

class Scene {
  public:
	virtual bool init() = 0;
	virtual bool onEnter() = 0;

	virtual bool step() = 0;
	virtual bool draw(uint64_t dt) const = 0;

	virtual bool onExit() = 0;

  public:
	virtual ~Scene() = default;
};

}  // namespace Bloodlust

#endif
