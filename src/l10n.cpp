#include "../include/l10n.hpp"

#include "../include/assets.hpp"
#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>
#include <cstdint>
#include <string>
#include <unordered_map>

using namespace Bloodlust;

using localeData = std::unordered_map<std::string, std::string>;

static std::unordered_map<std::string, localeData> translations = {};

bool L10n::loadLocale(const std::string& path) {
	uint64_t size = 0;
	void* data;
	SDL_IOStream* stream = Assets::getAsset(path, &data, &size);

	if (stream == nullptr) {
		return false;
	} else if (size == 0) {
		Logging::setError("Localisation table is empty");
		SDL_CloseIO(stream);
		SDL_free(data);
		return false;
	}

	std::string currentKey = "";
	std::string currentString = "";
	localeData localeMap = {};
	char c, lastChar = '\0';
	bool isParsingKey = true, trimSpaces = false;

	while (SDL_ReadU8(stream, (uint8_t*) &c)) {
		if (c == '\n') {
			if (isParsingKey && lastChar != '\n' && lastChar != '\0') {
				Logging::setError("Syntax error: expected localisation string, got newline");
				goto fail;
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
		Logging::setError("Syntax error: expected translation key, got end of file");
		goto fail;
	} else if (SDL_GetIOStatus(stream) != SDL_IO_STATUS_EOF) {
		goto fail;
	}

	SDL_CloseIO(stream);
	SDL_free(data);
	translations[path] = localeMap;
	return true;

fail:
	SDL_CloseIO(stream);
	SDL_free(data);
	return false;
}

const std::string L10n::getString(const std::string& locale, const std::string& key) {
	if (translations.contains(locale)) {
		if (translations[locale].contains(key)) {
			return translations[locale][key];
		}
		SDL_SetError("key %s does not exist", key.c_str());
		return "<ERROR>";
	}
	SDL_SetError("locale %s does not exist", locale.c_str());
	return "<ERROR>";
}
