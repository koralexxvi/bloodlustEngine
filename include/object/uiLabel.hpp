#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/font.hpp"
#include "../util.hpp"
#include "uiElement.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Bloodlust {

// A label that renders text.
class UILabel : public UIElement {
  private:
	// The text to be displayed by this label.
	std::string text = "-NO TEXT-";

	// A pointer to the internal text object this label holds.
	TTF_Text* textObject = nullptr;

	// A pointer to the internal TTF_Font this label holds.
	TTF_Font* textFont = nullptr;

	// A pointer to the font this label holds.
	Font* font = nullptr;

	// The style of this label's text.
	TTF_FontStyleFlags style = TTF_STYLE_NORMAL;

	// The horizontal alignment of this label's text.
	AlignModeHorizontal fontAlignH = AlignModeHorizontal::CENTER;

	// The vertical alignment of this label's text.
	AlignModeVertical fontAlignV = AlignModeVertical::CENTER;

	// The last draw rect that this label' text bounds was calculated with.
	mutable SDL_FRect lastDrawRect;

	// The cached bounds of this label's text.
	mutable SDL_FRect cachedTextBounds;

	// The cached draw size of this label's text.
	mutable float cachedFontDrawSize;

	// The size of this label's text. Ignored if scaleText is true.
	float fontSize = 12.0f;

  public:
	// The colour of this label's text.
	Colour textColour = {1.0f, 1.0f, 1.0f};

  private:
	// If true, the label's text will be scaled to its maximum bounds. Implies wrapText.
	bool scaleText = true;

	// If true, the label's text will be wrapped to its bounds. Newlines are always wrapped.
	bool wrapText = true;

	// True if this label's font needs to be updated.
	bool fontDirty = false;

	// True if the bounds of this label's text need to be recalculated.
	mutable bool textBoundsDirty = true;

  public:
	// Constructs a UILabel with a pointer to the given font. The pointer must not be null.
	UILabel(Font* initialFont);

	// Creates a copy of this label.
	std::unique_ptr<UILabel> copy() const;

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
		return fontSize;
	}

	// Sets the font size of this label.
	inline void setFontSize(float newSize) {
		fontSize = newSize;
		fontDirty = true;
		textBoundsDirty = true;
	}

	// Returns whether the text is automatically scaled to its bounds.
	constexpr bool getTextScaled() const {
		return scaleText;
	}

	// Sets whether the text is automatically scaled to its bounds. If set to true, then
	// getTextWrapped() will also return true.
	inline void setTextScaled(bool shouldScale) {
		scaleText = shouldScale;
		wrapText = shouldScale || wrapText;
		textBoundsDirty = true;
	}

	// Returns whether the text is currently wrapped to its bounds. This will always return true if
	// the text is also scaled to its boudns.
	constexpr bool getTextWrapped() const {
		return wrapText;
	}

	// Sets whether the text is automatically wrapped to its bounds. Ignored if the text is also
	// scaled to its bounds.
	inline void setTextWrapped(bool shouldWrap) {
		wrapText = scaleText || shouldWrap;
		textBoundsDirty = true;
	}

	// Returns a copy of this label's text.
	constexpr std::string getText() const {
		return text;
	}

	// Sets the text of this label.
	void setText(const std::string& str);

	// Returns the style of this label's font.
	constexpr TTF_FontStyleFlags getFontStyle() const {
		return style;
	}

	// Sets the style of this label's font.
	inline void setFontStyle(TTF_FontStyleFlags newStyle) {
		style = newStyle;
		fontDirty = true;
		textBoundsDirty = true;
	}

	// Returns the horizontal alignment of this label's font.
	constexpr AlignModeHorizontal getFontAlignH() const {
		return fontAlignH;
	}

	// Sets the horizontal alignment of this label's font.
	inline void setFontAlignH(AlignModeHorizontal alignH) {
		fontAlignH = alignH;
		fontDirty = true;
		textBoundsDirty = true;
	}

	// Returns the vertical alignment of this label's font.
	constexpr AlignModeVertical getFontAlignV() const {
		return fontAlignV;
	}

	// Sets the vertical alignment of this label's font.
	inline void setFontAlignV(AlignModeVertical alignV) {
		fontAlignV = alignV;
		textBoundsDirty = true;
	}

  private:
	// Returns the bounds of this label's text.
	SDL_FRect getTextBounds(const SDL_FRect& drawRect, float& fontDrawSize) const;

	// Updates the text if needed.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool updateText();
};

}  // namespace Bloodlust
