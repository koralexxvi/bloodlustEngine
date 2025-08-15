#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <cstdarg>

const char* Bloodlust::getError() {
	return SDL_GetError();
}

void Bloodlust::setError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	SDL_SetErrorV(fmt, args);
	va_end(args);
}

void Bloodlust::log(SDL_LogPriority priority, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_CUSTOM, priority, fmt, args);
	va_end(args);
}

void Bloodlust::log(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_INFO, fmt, args);
	va_end(args);
}
