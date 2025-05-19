#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::Font;

Font::Font(const char* fontPath, float pointSize) {
	fontData = TTF_OpenFont(fontPath, pointSize);
}

Font::Font(TTF_Font* fontData) : fontData(fontData) {}

bool Font::isValid() const {
	return fontData != nullptr;
}

TTF_Font* Font::getData() const {
	return fontData;
}

Font::~Font() {
	TTF_CloseFont(fontData);
}
