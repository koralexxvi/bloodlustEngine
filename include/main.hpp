#pragma once

#include <SDL3/SDL_init.h>

namespace Bloodlust {

// Exits the program, with an optional exit code (defaults to 0).
[[noreturn]]
void quit(int exitCode = 0);

}  // namespace Bloodlust
