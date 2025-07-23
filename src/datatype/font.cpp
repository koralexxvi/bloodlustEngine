#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>

using Bloodlust::Font;

int Font::getNearestFontSize(float size) {
	// TODO: surely there's a better way to do this
	if (size <= 6.0f) {
		return 6;
	} else if (size <= 12.0f) {
		return int(ceilf(size));
	} else if (size <= 28.0f) {
		return int(ceilf(size / 2.0f)) * 2;
	} else if (size <= BL_MAX_FONT_SIZE) {
		return int(ceilf(size / 12.0f)) * 12;
	}
	return BL_MAX_FONT_SIZE;
}

int Font::getFontSizeIndex(float size) {
	if (size <= 6.0f) {
		return 0;
	} else if (size <= 12.0f) {	 // 1-6
		return int(ceilf(size)) - 6;
	} else if (size <= 28.0f) {	 // 7-14
		return int(ceilf(size / 2.0f));
	} else if (size <= BL_MAX_FONT_SIZE) {	// 15-20
		return 12 + int(ceilf(size / 12.0f));
	}
	return 20;
}

Font::Font(const char* fontPath, bool& success) {
	fontData[0] = TTF_OpenFont(fontPath, 6.0f);
	success = fontData[0] != nullptr;
}

Font::Font(TTF_Font* font) {
	TTF_SetFontSize(font, 6.0f);
	fontData[0] = font;
}

TTF_Font* Font::getData(float height) {
	int index = getFontSizeIndex(height);
	if (fontData[index] == nullptr) {
		TTF_Font* newFont = TTF_CopyFont(fontData[0]);
		if (newFont == nullptr) {
			return nullptr;
		}
		TTF_SetFontSize(newFont, float(getNearestFontSize(height)));
		fontData[index] = newFont;
	}
	return fontData[index];
}

Bloodlust::Vector2 Font::getStringSize(const char* str, float height, int maxLen) {
	int w, h;
	TTF_Font* font = getData(height);
	if (font == nullptr) {
		return {-1.0f, -1.0f};
	} else if (!TTF_GetStringSizeWrapped(font, str, 0, maxLen, &w, &h)) {
		return {-1.0f, -1.0f};
	}

	float scale = height / float(TTF_GetFontHeight(font));
	return {float(w) * scale, float(h) * scale};
}

Font::~Font() {
	for (TTF_Font* font : fontData) {
		TTF_CloseFont(font);
	}
}
