#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>

using Bloodlust::Font;

float Font::getNearestFontSize(float size) {
	// TODO: surely there's a better way to do this
	if (size <= 6.0f) {
		return 6.0f;
	} else if (size <= 12.0f) {
		return roundf(size);
	} else if (size <= 28.0f) {
		return roundf(size / 2.0f) * 2.0f;
	} else if (size <= 96.0f) {
		return roundf(size / 12.0f) * 12.0f;
	}
	return 96.0f;
}

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
