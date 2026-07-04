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

std::string defaultLocale = "en_GB";

}  // namespace

bool Bloodlust::loadLocale(std::string_view path) noexcept {
	std::uint64_t size = 0;
	std::unique_ptr<std::uint8_t[]> data = getAsset(path, size);
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
	std::uint8_t c, lastChar = '\0';
	bool isParsingKey = true, trimSpaces = false;

	while (SDL_ReadU8(stream, &c)) {
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
				currentKey += static_cast<char>(c);
			} else {
				currentString += static_cast<char>(c);
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
	translations[std::string(path)] = localeMap;

cleanup:
	SDL_CloseIO(stream);
	return false;
}

const std::string Bloodlust::getDefaultLocale() noexcept {
	return defaultLocale;
}

void Bloodlust::setDefaultLocale(std::string_view locale) noexcept {
	defaultLocale = locale;
}

const std::string Bloodlust::getL10nString(std::string_view locale, std::string_view key) noexcept {
	std::string localeStr = std::string(locale);
	if (translations.contains(localeStr)) {
		std::string keyStr = std::string(key);
		if (translations[localeStr].contains(keyStr)) {
			return translations[localeStr][keyStr];
		}
		if (translations[defaultLocale].contains(keyStr)) {
			return translations[defaultLocale][keyStr];
		}
		setError("key %s does not exist", keyStr.c_str());
		return "-NO L10N KEY-";
	}
	setError("locale %s does not exist", localeStr.c_str());
	return "-NO LOCALE-";
}
