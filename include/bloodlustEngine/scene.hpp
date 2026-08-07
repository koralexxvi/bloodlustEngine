#pragma once

#include <SDL3/SDL_init.h>
#include <cstdint>

namespace Bloodlust {

struct InputEvent;

struct Scene {
	virtual ~Scene() noexcept = default;

	// A callback that is fired when the scene is entered, after the last scene's onEnter callback
	// is fired.
	virtual void onEnter();

	// A callback that is fired at the star
	virtual void step();

	// A callback that is fired at the star
	virtual void draw(std::uint64_t dt);

	// A callback that is fired on each inp
	virtual void onInput(const InputEvent& input);

	// A callback that is fired when the scene is exited, but before the new scene's onEnter
	// callback is fired.
	virtual void onExit();
};

}  // namespace Bloodlust
