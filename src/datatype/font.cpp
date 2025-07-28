#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::Font;

void Font::setFontStyle(TTF_FontStyleFlags style) {
	TTF_Font* font;

#define BL_CREATE_FONT_STYLE(align)                  \
	font = fontData[style | unsigned((align) << 2)]; \
	TTF_SetFontStyle(font, style);                   \
	TTF_SetFontWrapAlignment(font, (align));         \
	TTF_SetFontSDF(font, true);

	BL_CREATE_FONT_STYLE(TTF_HORIZONTAL_ALIGN_LEFT)
	BL_CREATE_FONT_STYLE(TTF_HORIZONTAL_ALIGN_CENTER)
	BL_CREATE_FONT_STYLE(TTF_HORIZONTAL_ALIGN_RIGHT)
#undef BL_CREATE_FONT_STYLE
}

Bloodlust::Vector2 Font::getStringSize(const char* str, TTF_Font* font, float height, int maxLen) {
	int w, h;
	float fontScale = height / BL_FONT_SIZE;
	if (TTF_GetStringSizeWrapped(font, str, 0, int(float(maxLen) / fontScale), &w, &h)) {
		return {float(w) * fontScale, float(h) * fontScale};
	}
	return {-1.0f, -1.0f};
}

Font::Font(const char* fontPath, bool& success) {
	fontData[0] = TTF_OpenFont(fontPath, BL_FONT_SIZE);
	success = fontData[0] != nullptr;
	if (!success) {
		return;
	}
	for (size_t i = 1; i < sizeof(fontData) / sizeof(*fontData); i++) {
		fontData[i] = TTF_CopyFont(fontData[0]);
		if (fontData[i] == nullptr) {
			success = false;
			return;
		}
	}
	setFontStyle(TTF_STYLE_NORMAL);
	setFontStyle(TTF_STYLE_BOLD);
	setFontStyle(TTF_STYLE_ITALIC);
	setFontStyle(TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
}

TTF_Font* Font::getData(TTF_FontStyleFlags style, TTF_HorizontalAlignment align) const {
	return fontData[style | unsigned(align << 2)];
}

Font::~Font() {
	for (TTF_Font* font : fontData) {
		TTF_CloseFont(font);
	}
}
