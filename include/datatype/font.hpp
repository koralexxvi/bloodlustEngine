#pragma once

#include "../../include/object/uiElement.hpp"
#include "../util.hpp"
#include "vector2.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#define BL_MAX_FONT_SIZE (96.0f)

namespace Bloodlust {

class Font {
  private:
	TTF_Font* fontData[12] = {};

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool createFontStyle(TTF_FontStyleFlags style);

  public:
	static constexpr TTF_HorizontalAlignment alignHToTTFAlign(AlignModeHorizontal alignH) {
		switch (alignH) {
			case AlignModeHorizontal::LEFT: return TTF_HORIZONTAL_ALIGN_LEFT;
			case AlignModeHorizontal::CENTER: return TTF_HORIZONTAL_ALIGN_CENTER;
			case AlignModeHorizontal::RIGHT: return TTF_HORIZONTAL_ALIGN_RIGHT;
			default: return TTF_HORIZONTAL_ALIGN_INVALID;
		}
	}

	static Vector2 getStringSize(const char* str, TTF_Font* font, float height, int maxLen = 0);

	Font(const char* fontPath, bool& success);

	TTF_Font* getData(TTF_FontStyleFlags style, TTF_HorizontalAlignment align) const;

	~Font();
};

}  // namespace Bloodlust
