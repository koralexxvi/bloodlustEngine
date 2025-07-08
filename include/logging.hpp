#pragma once

#include <SDL3/SDL_log.h>
#include <string>

namespace Bloodlust {

void setError(const char* errMsg);
const char* getError();

void log(const std::string& msg, SDL_LogPriority priority = SDL_LOG_PRIORITY_INFO);

}  // namespace Bloodlust
