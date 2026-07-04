#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::Font;

void Font::setFontStyle(TTF_FontStyleFlags style) noexcept {
	TTF_Font* font;

	auto createFontStyle = [&](TTF_HorizontalAlignment align) {
		font = fontData[style | static_cast<std::uint32_t>((align) << 2)];
		TTF_SetFontStyle(font, style);
		TTF_SetFontWrapAlignment(font, align);
		// TTF_SetFontSDF(font, true);
	};

	createFontStyle(TTF_HORIZONTAL_ALIGN_LEFT);
	createFontStyle(TTF_HORIZONTAL_ALIGN_CENTER);
	createFontStyle(TTF_HORIZONTAL_ALIGN_RIGHT);
}

Bloodlust::Vector2 Font::getStringSize(
	const char* str,
	TTF_Font* font,
	float height,
	float maxLen
) noexcept {
	int w, h;
	float fontScale = height / BL_FONT_SIZE;
	if (TTF_GetStringSizeWrapped(
			font,
			str,
			0,
			static_cast<int>(SDL_ceilf(maxLen / fontScale)),
			&w,
			&h
		))
	{
		return {static_cast<float>(w) * fontScale, static_cast<float>(h) * fontScale};
	}
	return {-1.0f, -1.0f};
}

Font::Font(const char* fontPath, bool& success) noexcept {
	fontData[0] = TTF_OpenFont(fontPath, BL_FONT_SIZE);
	success = fontData[0] != nullptr;
	if (!success) {
		Bloodlust::log("failed %s", getError().c_str());
		return;
	}
	for (std::size_t i = 1; i < fontData.size(); i++) {
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

TTF_Font* Font::getData(TTF_FontStyleFlags style, TTF_HorizontalAlignment align) const noexcept {
	return fontData[style | static_cast<std::uint32_t>(align << 2)];
}

Font::~Font() noexcept {
	for (TTF_Font* font : fontData) {
		TTF_CloseFont(font);
	}
}
