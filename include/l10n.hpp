#ifndef BL_L10N_H
#define BL_L10N_H

#include <string>

namespace Bloodlust {

bool loadLocale(const std::string& path);

const std::string getL10nString(const std::string& locale, const std::string& key);

}  // namespace Bloodlust

#endif
