#ifndef BL_I18N_H
#define BL_I18N_H

#include <string>

namespace Bloodlust::L10n {

bool loadLocale(const std::string& path);

const std::u16string getString(const std::string& locale, const std::u16string& key);

}

#endif
