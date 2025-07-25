#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::Font;

bool Font::createFontStyle(TTF_FontStyleFlags style) {
	TTF_Font* base = fontData[0];
	TTF_Font* newFont;

#define BL_CREATE_FONT_STYLE(align)             \
	newFont = TTF_CopyFont(base);               \
	if (newFont == nullptr) {                   \
		return false;                           \
	}                                           \
	TTF_SetFontStyle(newFont, style);           \
	TTF_SetFontWrapAlignment(newFont, (align)); \
	fontData[style | ((align) << 2)] = newFont;

	if (style != TTF_STYLE_NORMAL) {
		BL_CREATE_FONT_STYLE(TTF_HORIZONTAL_ALIGN_LEFT)
	}
	BL_CREATE_FONT_STYLE(TTF_HORIZONTAL_ALIGN_CENTER)
	BL_CREATE_FONT_STYLE(TTF_HORIZONTAL_ALIGN_RIGHT)
#undef BL_CREATE_FONT_STYLE
	return true;
}

Bloodlust::Vector2 Font::getStringSize(const char* str, TTF_Font* font, float height, int maxLen) {
	int w, h;
	if (font == nullptr) {
		return {-1.0f, -1.0f};
	} else if (!TTF_GetStringSizeWrapped(font, str, 0, maxLen, &w, &h)) {
		return {-1.0f, -1.0f};
	}

	float scale = height / float(TTF_GetFontHeight(font));
	return {float(w) * scale, float(h) * scale};
}

Font::Font(const char* fontPath, bool& success) {
	fontData[0] = TTF_OpenFont(fontPath, 12.0f);
	success = fontData[0] != nullptr && TTF_SetFontSDF(fontData[0], true)
		   && createFontStyle(TTF_STYLE_NORMAL) && createFontStyle(TTF_STYLE_BOLD)
		   && createFontStyle(TTF_STYLE_ITALIC)
		   && createFontStyle(TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
}

TTF_Font* Font::getData(TTF_FontStyleFlags style, TTF_HorizontalAlignment align) const {
	return fontData[style | unsigned(align << 2)];
}

Font::~Font() {
	for (TTF_Font* font : fontData) {
		TTF_CloseFont(font);
	}
}
