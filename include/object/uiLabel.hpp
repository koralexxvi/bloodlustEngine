#pragma once

#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "../util.hpp"
#include "uiElement.hpp"

#include <SDL3/SDL_rect.h>
#include <string>

namespace Bloodlust {

class UILabel : public UIElement {
  private:
	Font* font;
	float fontSize = -1.0f;

	[[maybe_unused]]
	char _padding[4];

  public:
	std::string text = "-NO TEXT-";
	Color textColor = {1.0f, 1.0f, 1.0f};
	AlignModeHorizontal fontAlignH = AlignModeHorizontal::CENTER;
	AlignModeVertical fontAlignV = AlignModeVertical::CENTER;

	UILabel(Font* initialFont);

	[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
	UILabel* copy() const;

	bool canDraw() const override;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() override;

	void setFont(Font* font);
	void setFontSize(float newSize);

  private:
	SDL_FRect getTextBounds(const SDL_FRect& drawRect) const;
};

}  // namespace Bloodlust
