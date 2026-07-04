#pragma once

#include "../object/uiElement.hpp"
#include "vector2.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#ifndef BL_FONT_SIZE
	#define BL_FONT_SIZE (64.0f)
#endif

namespace Bloodlust {

// A class representing a font face.
class Font {
  private:
	std::array<TTF_Font*, 12> fontData = {};

  public:
	// Converts an AlignModeHorizontal enum to a TTF_HorizontalAlignment enum.
	static constexpr TTF_HorizontalAlignment alignHToTTFAlign(AlignModeHorizontal alignH) noexcept {
		switch (alignH) {
			case AlignModeHorizontal::LEFT: return TTF_HORIZONTAL_ALIGN_LEFT;
			case AlignModeHorizontal::CENTER: return TTF_HORIZONTAL_ALIGN_CENTER;
			case AlignModeHorizontal::RIGHT: return TTF_HORIZONTAL_ALIGN_RIGHT;
			default: return TTF_HORIZONTAL_ALIGN_INVALID;
		}
	}

	// Returns the size that a string would take up given a string, font and height.
	// If maxLen is greater than 0.0f, then the string will wrap at that point. Newlines are always
	// wrapped.
	static Vector2 getStringSize(
		const char* str,
		TTF_Font* font,
		float height,
		float maxLen = 0.0f
	) noexcept;

	// Constructs a new font from the given path, and fills success with true if successful.
	Font(const char* fontPath, bool& success) noexcept;

	// Returns the TTF_Font struct associated with the given style and alignment.
	TTF_Font* getData(TTF_FontStyleFlags style, TTF_HorizontalAlignment align) const noexcept;

	// Destroys this font, freeing each TTF_Font associated with it. All pointers to TTF_Font
	// structs associated with this will be closed and now be invalid.
	~Font() noexcept;

  private:
	// Sets the font style of this font face.
	void setFontStyle(TTF_FontStyleFlags style) noexcept;
};

}  // namespace Bloodlust
