#include "../include/l10n.hpp"

#include "../include/assets.hpp"
#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>

using namespace Bloodlust;

static std::unordered_map<std::string, std::map<std::u16string, std::u16string>> translations = {};

bool L10n::loadLocale(const std::string& path) {
	uint64_t size;
	void* data = nullptr;
	bool success = false;
	SDL_IOStream* stream = Assets::getAsset(path, &data, &size);
	if (stream == nullptr) {
		return false;
	} else if (size == 0) {
		SDL_SetError("Localisation table is empty");
		SDL_CloseIO(stream);
		SDL_free(data);
		return false;
	}

	uint16_t c = '\0';
	uint16_t lastChar = '\0';
	bool isParsingKey = false;
	bool trimSpaces = false;
	std::u16string currentKey = {};
	std::u16string currentString = {};

	while (SDL_ReadU16LE(stream, &c)) {
		if (c == ':') {
			currentString.push_back(':');
			if (isParsingKey) {
				isParsingKey = false;
				trimSpaces = true;
			}
		} else if (c == '\n') {
			if (!isParsingKey) {
				Logging::setError("Syntax error: expected string, got newline");
				break;
			} else if (lastChar == '\\') {
				continue;
			}
			currentString.push_back('\n');
			isParsingKey = true;
		} else {
			if (c == ' ' && trimSpaces) {
				continue;
			}
			trimSpaces = false;
			if (isParsingKey) {
				currentKey.push_back(c);
			} else {
				currentString.push_back(c);
			}
		}
		lastChar = c;
	}
	success = SDL_GetIOStatus(stream) == SDL_IO_STATUS_EOF;

	SDL_CloseIO(stream);
	SDL_free(data);
	return success;
}

const std::u16string L10n::getString(const std::string& locale, const std::u16string& key) {
	return translations[locale][key];
}
