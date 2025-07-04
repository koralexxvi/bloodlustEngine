#pragma once

#include <SDL3/SDL_init.h>

namespace Bloodlust {

SDL_AppResult init(int argcount, char* args[]);
SDL_AppResult processEvent(SDL_Event* event);
SDL_AppResult iterate();
void quit(SDL_AppResult result);

}  // namespace Bloodlust
