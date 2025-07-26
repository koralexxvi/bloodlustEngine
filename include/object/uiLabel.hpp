#pragma once

#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "../util.hpp"
#include "uiElement.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Bloodlust {

class UILabel : public UIElement {
  private:
	std::string text = "-NO TEXT-";
	TTF_Text* textObject = nullptr;
	TTF_Font* textFont = nullptr;
	Font* font = nullptr;
	TTF_FontStyleFlags style = TTF_STYLE_NORMAL;
	AlignModeHorizontal fontAlignH = AlignModeHorizontal::CENTER;
	AlignModeVertical fontAlignV = AlignModeVertical::CENTER;

  public:
	Color textColor = {1.0f, 1.0f, 1.0f};
	float fontSize = 12.0f;

	bool scaleText = true;
	bool wrapText = false;

  private:
	bool fontDirty = false;

	[[maybe_unused]]
	char _padding[5];

  public:
	UILabel(Font* initialFont);

	[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
	UILabel* copy() const;

	bool isTextVisible() const;
	bool canDraw() const override;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() override;

	void setFont(Font* font);

	std::string getText() const;
	void setText(const std::string& str);

	TTF_FontStyleFlags getFontStyle() const;
	void setFontStyle(TTF_FontStyleFlags newStyle);

	AlignModeHorizontal getFontAlignH() const;
	void setFontAlignH(AlignModeHorizontal alignMode);
	AlignModeVertical getFontAlignV() const;
	void setFontAlignV(AlignModeVertical alignMode);

  private:
	SDL_FRect getTextBounds(const SDL_FRect& drawRect) const;
};

}  // namespace Bloodlust
