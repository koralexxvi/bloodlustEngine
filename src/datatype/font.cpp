#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::DataTypes::Font;

Font::Font(const char* fontPath, float pointSize) {
	fontData = TTF_OpenFont(fontPath, pointSize);
}

bool Font::isValid() const {
	return fontData != nullptr;
}

TTF_Font* Font::getData() const {
	return fontData;
}

Font::~Font() {
	TTF_CloseFont(fontData);
}
