#pragma once

#include "../object/uiElement.hpp"
#include "vector2.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#ifndef BL_FONT_SIZE
	#define BL_FONT_SIZE (64.0f)
#endif

namespace Bloodlust {

class Font {
  private:
	std::array<TTF_Font*, 12> fontData = {};

	void setFontStyle(TTF_FontStyleFlags style);

  public:
	static constexpr TTF_HorizontalAlignment alignHToTTFAlign(AlignModeHorizontal alignH) {
		switch (alignH) {
			case AlignModeHorizontal::LEFT: return TTF_HORIZONTAL_ALIGN_LEFT;
			case AlignModeHorizontal::CENTER: return TTF_HORIZONTAL_ALIGN_CENTER;
			case AlignModeHorizontal::RIGHT: return TTF_HORIZONTAL_ALIGN_RIGHT;
			default: return TTF_HORIZONTAL_ALIGN_INVALID;
		}
	}

	static Vector2 getStringSize(
		const char* str,
		TTF_Font* font,
		float height,
		float maxLen = 0.0f
	);

	Font(const char* fontPath, bool& success);

	TTF_Font* getData(TTF_FontStyleFlags style, TTF_HorizontalAlignment align) const;

	~Font();
};

}  // namespace Bloodlust
