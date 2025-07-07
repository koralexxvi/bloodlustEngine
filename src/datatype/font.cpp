#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>

using Bloodlust::Font;

Font::Font(const char* fontPath, float pointSize) : pointSize(roundf(pointSize)) {
	fontData = TTF_OpenFont(fontPath, this->pointSize);
}

Font::Font(TTF_Font* fontData) : fontData(fontData) {}

bool Font::isValid() const {
	return fontData != nullptr;
}

TTF_Font* Font::getData() const {
	return fontData;
}

float Font::getPointSize() const {
	return pointSize;
}

bool Font::rescaleTo(float newSize) {
	pointSize = roundf(newSize);
	return TTF_SetFontSize(fontData, newSize);
}

Font::~Font() {
	TTF_CloseFont(fontData);
}
