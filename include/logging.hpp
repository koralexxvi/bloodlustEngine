#pragma once

#include <SDL3/SDL_log.h>
#include <format>
#include <string>

#if defined(__GNUC__) && __has_attribute(format)
	#define BL_FORMAT_STRING(start, end) gnu::format(printf, start, end)
#else
	#define BL_FORMAT_STRING(start, end)
#endif

namespace Bloodlust {

// Returns a copy of the current error string.
std::string getError() noexcept;

// Sets the current error string to the given format string.
[[BL_FORMAT_STRING(1, 2)]]
void setError(const char* fmt, ...) noexcept;

void log(std::string_view msg);

// Logs the given format string to stdout.
template<typename... Args> void log(std::format_string<Args...> fmt, Args&&... args) noexcept {
	std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
	SDL_LogMessage(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_INFO, "%s", str.c_str());
}

// Logs the format string to stdout with the given priority.
template<typename... Args>
void log(SDL_LogPriority priority, std::format_string<Args...> fmt, Args&&... args) noexcept {
	std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
	SDL_LogMessage(SDL_LOG_CATEGORY_CUSTOM, priority, "%s", str.c_str());
}

[[noreturn]]
void reportFailure(std::string_view errMsg, bool noWindow = false) noexcept;

}  // namespace Bloodlust
