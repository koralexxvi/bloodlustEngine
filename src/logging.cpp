#include "../include/logging.hpp"

#include <SDL3/SDL_log.h>
#include <string>

using namespace Bloodlust;

static std::string err = "<ERROR MESSAGE UNSET>";

const std::string Logging::getError() {
	return err;
}

void Logging::setError(const std::string& errMsg) {
	err = std::string(errMsg);
}

static constexpr SDL_LogPriority getSDLPriority(Logging::LogPriority priority) {
	using enum Logging::LogPriority;
	switch (priority) {
		case TRACE: return SDL_LOG_PRIORITY_TRACE;
		case VERBOSE: return SDL_LOG_PRIORITY_VERBOSE;
		case DEBUG: return SDL_LOG_PRIORITY_DEBUG;
		case INFO: return SDL_LOG_PRIORITY_INFO;
		case WARNING: return SDL_LOG_PRIORITY_WARN;
		case ERROR: return SDL_LOG_PRIORITY_ERROR;
		case CRITICAL: return SDL_LOG_PRIORITY_CRITICAL;
		default: return SDL_LOG_PRIORITY_INVALID;
	}
}

void Logging::log(const std::string& msg, LogPriority priority) {
	SDL_LogMessage(SDL_LOG_CATEGORY_CUSTOM, getSDLPriority(priority), "%s", msg.c_str());
}
