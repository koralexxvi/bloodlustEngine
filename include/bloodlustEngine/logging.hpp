#pragma once

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <format>
#include <string>

namespace Bloodlust {

// Returns a copy of the current error string.
std::string getError() noexcept;

// Sets the current error string to the given format string.
template<typename... Args> void setError(std::format_string<Args...> fmt, Args&&... args) noexcept {
	std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
	SDL_SetError("%s", msg.c_str());
}

// Logs the given string to stdout.
void log(std::string_view msg);

// Logs the given string to stdout with the given priority.
void log(SDL_LogPriority priority, std::string_view msg);

// Logs the given string to stdout with std::format's formatting.
template<typename... Args> void log(std::format_string<Args...> fmt, Args&&... args) noexcept {
	std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
	SDL_LogMessage(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_INFO, "%s", str.c_str());
}

// Logs the string to stdout with the given priority.
template<typename... Args>
void log(SDL_LogPriority priority, std::format_string<Args...> fmt, Args&&... args) noexcept {
	std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
	SDL_LogMessage(SDL_LOG_CATEGORY_CUSTOM, priority, "%s", str.c_str());
}

// Reports an error message and exits, showing an error window unless noWindow is true.
[[noreturn]]
void reportFailure(std::string_view errMsg, bool noWindow = false) noexcept;

}  // namespace Bloodlust
