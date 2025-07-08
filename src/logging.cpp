#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <string>

const char* Bloodlust::getError() {
	return SDL_GetError();
}

void Bloodlust::setError(const char* errMsg) {
	SDL_SetError("%s", errMsg);
}

void Bloodlust::log(const std::string& msg, SDL_LogPriority priority) {
	SDL_LogMessage(SDL_LOG_CATEGORY_CUSTOM, priority, "%s", msg.c_str());
}
