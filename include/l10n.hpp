#pragma once

#include "util.hpp"

#include <string>

namespace Bloodlust {

// Loads a locale from the given path into memory.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool loadLocale(const char* path);

// Returns the default locale; defaults to en-us.
const std::string getDefaultLocale();

// Sets the default locale.
void setDefaultLocale(const std::string& locale);

// Returns a localised string for the given key. If not found in this locale, tries to get it from
// the default locale, otherwise returns an error message.
const std::string getL10nString(const std::string& locale, const std::string& key);

}  // namespace Bloodlust
