#pragma once

#include "util.hpp"

#include <string>
#include <string_view>

namespace Bloodlust {

// Loads a locale from the given path into memory.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool loadLocale(std::string_view path) noexcept;

// Returns the default locale; defaults to en_GB.
const std::string getDefaultLocale() noexcept;

// Sets the default locale.
void setDefaultLocale(std::string_view locale) noexcept;

// Returns a localised string for the given key. If not found in this locale, tries to get it from
// the default locale, otherwise returns an error message.
const std::string getL10nString(std::string_view locale, std::string_view key) noexcept;

}  // namespace Bloodlust
