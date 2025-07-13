#pragma once

#include <string>

namespace Bloodlust {

bool loadLocale(const char* path);

const std::string getDefaultLocale();
void setDefaultLocale(const std::string& locale);

const std::string getL10nString(const std::string& locale, const std::string& key);

}  // namespace Bloodlust
