#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/font.hpp"
#include "../util.hpp"
#include "uiElement.hpp"

#include <SDL3/SDL_rect.h>

namespace Bloodlust {

// A label that renders text.
class UILabel : public UIElement {
  public:
	// The colour of this label's text.
	Colour textColour = {1.0f, 1.0f, 1.0f};

	// Constructs a UILabel with a pointer to the given font. The pointer must not be null.
	UILabel(Font* initialFont);

	// Creates a copy of this label.
	[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
	UILabel* copy() const;

	// Returns true if this label's text can be drawn.
	bool isTextVisible() const;

	// Returns true if this label can be drawn.
	bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const override;

	// Draws this label.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() override;

	// Sets the font of this label. The pointer must not be null.
	void setFont(Font* font);

	// Returns the font size of this label.
	constexpr float getFontSize() const {
		return 8.0f;
	}

	// Sets the font size of this label.
	inline void setFontSize(float) {}

	// Returns whether the text is currently wrapped to its bounds.
	constexpr bool getTextWrapped() const {
		return false;
	}

	// Sets whether the text is automatically wrapped to its bounds.
	inline void setTextWrapped(bool) {}

	// Returns a copy of this label's text.
	constexpr std::string getText() const {
		return "";
	}

	// Sets the text of this label.
	void setText(const std::string& str);

	// Returns the horizontal alignment of this label's font.
	constexpr AlignModeHorizontal getFontAlignH() const {
		return AlignModeHorizontal::CENTER;
	}

	// Sets the horizontal alignment of this label's font.
	inline void setFontAlignH(AlignModeHorizontal) {}

	// Returns the vertical alignment of this label's font.
	constexpr AlignModeVertical getFontAlignV() const {
		return AlignModeVertical::CENTER;
	}

	// Sets the vertical alignment of this label's font.
	inline void setFontAlignV(AlignModeVertical) {}
};

}  // namespace Bloodlust
