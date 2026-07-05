#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_messagebox.h>
#include <bloodlustEngine/logging.hpp>
#include <format>

std::string Bloodlust::getError() noexcept {
	return std::string(SDL_GetError());
}

void Bloodlust::log(std::string_view msg) {
	SDL_Log("%s", std::string(msg).c_str());
}

void Bloodlust::log(SDL_LogPriority priority, std::string_view msg) {
	SDL_LogMessage(SDL_LOG_CATEGORY_CUSTOM, priority, "%s", std::string(msg).c_str());
}

[[noreturn]]
void Bloodlust::reportFailure(std::string_view errMsg, bool noWindow) noexcept {
	log(SDL_LOG_PRIORITY_ERROR, "A fatal error has occurred!");
	log(SDL_LOG_PRIORITY_ERROR, "Reason: {}", errMsg);
	if (!noWindow) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Fatal Error",
			std::format("A fatal error has occurred!\nReason: {}", std::string(errMsg).c_str())
				.c_str(),
			nullptr
		);
	}
	std::terminate();
}
