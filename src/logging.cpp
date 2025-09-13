#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_messagebox.h>
#include <cstdarg>
#include <format>

std::string Bloodlust::getError() {
	return std::string(SDL_GetError());
}

void Bloodlust::setError(const char* fmt, ...) {
	std::va_list args;
	va_start(args, fmt);
	SDL_SetErrorV(fmt, args);
	va_end(args);
}

void Bloodlust::log(SDL_LogPriority priority, const char* fmt, ...) {
	std::va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_CUSTOM, priority, fmt, args);
	va_end(args);
}

void Bloodlust::log(const char* fmt, ...) {
	std::va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_INFO, fmt, args);
	va_end(args);
}

[[noreturn]]
void Bloodlust::reportFailure(const std::string&& errMsg, bool noWindow) {
	log("A fatal error has occurred!");
	log("Reason: %s", errMsg.c_str());
	if (!noWindow) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Fatal Error",
			std::format("A fatal error has occurred!\nReason: {}", errMsg).c_str(),
			nullptr
		);
	}
	std::terminate();
}
