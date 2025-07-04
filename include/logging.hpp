#ifndef BL_LOGGING_H
#define BL_LOGGING_H

#include <string>

namespace Bloodlust {

enum class LogPriority {
	TRACE,
	VERBOSE,
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

void setError(const char* errMsg);
const char* getError();

void log(const std::string& msg, LogPriority priority = LogPriority::INFO);

}  // namespace Bloodlust

#endif
