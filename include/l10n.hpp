#pragma once

#include "util.hpp"

#include <string>

namespace Bloodlust {

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool loadLocale(const char* path);

const std::string getDefaultLocale();
void setDefaultLocale(const std::string& locale);

const std::string getL10nString(const std::string& locale, const std::string& key);

}  // namespace Bloodlust
