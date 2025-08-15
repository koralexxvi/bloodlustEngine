#pragma once

#include "../datatype/vector2.hpp"
#include "../util.hpp"

#include <SDL3/SDL_video.h>

namespace Bloodlust {

Vector2 getResolution();

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool setWindowed();

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool setBorderlessFullscreen(SDL_DisplayID display);

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool setExclusiveFullscreen(SDL_DisplayID display);

}  // namespace Bloodlust
