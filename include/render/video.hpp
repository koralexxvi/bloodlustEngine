#pragma once

#include "../datatype/vector2.hpp"
#include "../util.hpp"

#include <SDL3/SDL_video.h>

namespace Bloodlust {

// Gets the current window resolution.
Vector2 getResolution();

// Sets the fullscreen mode to windowed.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool setWindowed();

// Sets the fullscreen mode to borderless fullscreen on the given display.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool setBorderlessFullscreen(SDL_DisplayID display);

// Sets the fullscreen mode to exclusive fullscreen on the given display.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool setExclusiveFullscreen(SDL_DisplayID display);

}  // namespace Bloodlust
