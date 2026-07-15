#include <SDL3_ttf/SDL_ttf.h>
#include <bloodlustEngine/asset/font.hpp>
#include <bloodlustEngine/exceptions/assetLoadException.hpp>
#include <bloodlustEngine/exceptions/internalError.hpp>
#include <bloodlustEngine/logging.hpp>

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
	std::string_view str,
	TTF_Font* font,
	float height,
	float maxLen
) noexcept {
	int w, h;
	float fontScale = height / BL_FONT_SIZE;
	if (TTF_GetStringSizeWrapped(
			font,
			std::string(str).c_str(),
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

Font::Font(std::string_view fontPath) {
	fontData[0] = TTF_OpenFont(std::string(fontPath).c_str(), BL_FONT_SIZE);
	if (fontData[0] == nullptr) {
		throw AssetLoadException("Could not load font: " + getError(), fontPath);
	}
	for (std::size_t i = 1; i < fontData.size(); i++) {
		fontData[i] = TTF_CopyFont(fontData[0]);
		if (fontData[i] == nullptr) {
			throw InternalError("Could not create font style: " + getError());
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
