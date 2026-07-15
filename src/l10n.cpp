#include <SDL3/SDL_iostream.h>
#include <bloodlustEngine/assets.hpp>
#include <bloodlustEngine/exception/assetLoadException.hpp>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/l10n.hpp>
#include <bloodlustEngine/logging.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

using LocaleData = std::unordered_map<std::string, std::string>;

namespace {

std::unordered_map<std::string, LocaleData> translations = {};

std::string defaultLocale = "en_GB";

}  // namespace

void Bloodlust::loadLocale(std::string_view path) {
	std::uint64_t size = 0;
	std::unique_ptr<std::uint8_t[]> data = getAsset(path, size);
	SDL_IOStream* stream = SDL_IOFromConstMem(data.get(), size);
	if (stream == nullptr) {
		throw InternalError("Could not open stream for localisation table: " + getError());
	} else if (size == 0) {
		SDL_CloseIO(stream);
		throw AssetLoadException("Localisation table is empty", path);
	}

	std::string currentKey = "";
	std::string currentString = "";
	LocaleData localeMap = {};
	std::uint8_t c, lastChar = '\0';
	bool isParsingKey = true, trimSpaces = false;

	while (SDL_ReadU8(stream, &c)) {
		if (c == '\n') {
			if (isParsingKey && lastChar != '\n' && lastChar != '\0') {
				SDL_CloseIO(stream);
				throw AssetLoadException(
					"Syntax error: expected localisation string, got newline",
					path
				);
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
		SDL_CloseIO(stream);
		throw AssetLoadException("Syntax error: expected translation key, got end of file", path);
	} else if (SDL_GetIOStatus(stream) != SDL_IO_STATUS_EOF) {
		SDL_CloseIO(stream);
		throw InternalError("Could not read from localisation table stream");
	}
	translations[std::string(path)] = localeMap;
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
		setError("key {} does not exist", keyStr);
		return "-NO L10N KEY-";
	}
	setError("locale {} does not exist", localeStr);
	return "-NO LOCALE-";
}
