#pragma once

#include <SDL3/SDL_log.h>

#if defined(__GNUC__) && __has_attribute(format)
	#define BL_FORMAT_STRING(start, end) gnu::format(printf, start, end)
#else
	#define BL_FORMAT_STRING(start, end)
#endif

namespace Bloodlust {

const char* getError();

[[BL_FORMAT_STRING(1, 2)]]
void setError(const char* fmt, ...);

[[BL_FORMAT_STRING(1, 2)]]
void log(const char* fmt, ...);

[[BL_FORMAT_STRING(2, 3)]]
void log(SDL_LogPriority priority, const char* fmt, ...);

}  // namespace Bloodlust
