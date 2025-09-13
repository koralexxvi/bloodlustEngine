#pragma once

#include <SDL3/SDL_log.h>
#include <string>

#if defined(__GNUC__) && __has_attribute(format)
	#define BL_FORMAT_STRING(start, end) gnu::format(printf, start, end)
#else
	#define BL_FORMAT_STRING(start, end)
#endif

namespace Bloodlust {

// Returns a copy of the current error string.
std::string getError();

// Sets the current error string to the given format string.
[[BL_FORMAT_STRING(1, 2)]]
void setError(const char* fmt, ...);

// Logs the given format string to stdout.
[[BL_FORMAT_STRING(1, 2)]]
void log(const char* fmt, ...);

// Logs the format string to stdout with the given priority.
[[BL_FORMAT_STRING(2, 3)]]
void log(SDL_LogPriority priority, const char* fmt, ...);

[[noreturn]]
void reportFailure(const std::string&& errMsg, bool noWindow = false);

}  // namespace Bloodlust
