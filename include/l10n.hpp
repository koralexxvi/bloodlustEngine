#ifndef BL_I18N_H
#define BL_I18N_H

#include <string>

namespace Bloodlust::L10n {

bool loadLocale(const std::string& path);

const std::string getString(const std::string& locale, const std::string& key);

}  // namespace Bloodlust::L10n

#endif
