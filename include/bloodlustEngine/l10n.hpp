#pragma once

#include <string>
#include <string_view>

namespace Bloodlust {

// Loads a locale from the given path into memory.
// Throws AssetLoadException if the locale could not be loaded.
// Can also throw InternalError on failure.
void loadLocale(std::string_view path);

// Returns the default locale; defaults to en_GB.
const std::string getDefaultLocale() noexcept;

// Sets the default locale.
void setDefaultLocale(std::string_view locale) noexcept;

// Returns a localised string for the given key. If not found in this locale, tries to get it from
// the default locale, otherwise returns an error message.
const std::string getL10nString(std::string_view locale, std::string_view key) noexcept;

}  // namespace Bloodlust
