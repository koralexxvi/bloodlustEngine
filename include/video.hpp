#ifndef BL_VIDEO_H
#define BL_VIDEO_H

#include "datatype/vector2.hpp"

#include <SDL3/SDL_video.h>

namespace Bloodlust::Video {

DataTypes::Vector2 getResolution();

bool setWindowed();
bool setBorderlessFullscreen(SDL_DisplayID display);
bool setExclusiveFullscreen(SDL_DisplayID display);

}  // namespace Bloodlust::Video

#endif
