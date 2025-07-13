#include "../include/l10n.hpp"

#include "../include/assets.hpp"
#include "../include/logging.hpp"

#include <SDL3/SDL_iostream.h>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

using LocaleData = std::unordered_map<std::string, std::string>;

namespace {

std::unordered_map<std::string, LocaleData> translations = {};

std::string defaultLocale = "en-us";

}  // namespace

bool Bloodlust::loadLocale(const char* path) {
	uint64_t size = 0;
	std::unique_ptr<uint8_t> data = getAsset(path, size);
	if (data == nullptr) {
		return false;
	}
	SDL_IOStream* stream = SDL_IOFromConstMem(data.get(), size);
	if (stream == nullptr) {
		return false;
	} else if (size == 0) {
		setError("Localisation table is empty");
		SDL_CloseIO(stream);
		return false;
	}

	std::string currentKey = "";
	std::string currentString = "";
	LocaleData localeMap = {};
	char c, lastChar = '\0';
	bool isParsingKey = true, trimSpaces = false;

	while (SDL_ReadU8(stream, (uint8_t*) &c)) {
		if (c == '\n') {
			if (isParsingKey && lastChar != '\n' && lastChar != '\0') {
				setError("Syntax error: expected localisation string, got newline");
				goto cleanup;
			} else if (lastChar == '\\') {
				currentString.pop_back();
				continue;
			}
			localeMap[currentKey] = currentString;
			currentKey.clear();
			currentString.clear();
			isParsingKey = true;
		} else if (c == ':') {
			if (isParsingKey) {
				isParsingKey = false;
				trimSpaces = true;
			}
		} else {
			if (c == ' ' && trimSpaces) {
				continue;
			}
			trimSpaces = false;
			if (isParsingKey) {
				currentKey += c;
			} else {
				currentString += c;
			}
		}
		lastChar = c;
	}
	if (isParsingKey && lastChar != '\n') {
		setError("Syntax error: expected translation key, got end of file");
		goto cleanup;
	} else if (SDL_GetIOStatus(stream) != SDL_IO_STATUS_EOF) {
		goto cleanup;
	}
	translations[path] = localeMap;

cleanup:
	SDL_CloseIO(stream);
	return false;
}

const std::string Bloodlust::getDefaultLocale() {
	return defaultLocale;
}

void Bloodlust::setDefaultLocale(const std::string& locale) {
	defaultLocale = locale;
}

const std::string Bloodlust::getL10nString(const std::string& locale, const std::string& key) {
	if (translations.contains(locale)) {
		if (translations[locale].contains(key)) {
			return translations[locale][key];
		}
		if (translations[defaultLocale].contains(key)) {
			return translations[defaultLocale][key];
		}
		setError("key %s does not exist", key.c_str());
		return "-NO L10N KEY-";
	}
	setError("locale %s does not exist", locale.c_str());
	return "-NO LOCALE-";
}
