#pragma once

#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "uiElement.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <string>

namespace Bloodlust {

class UILabel : public UIElement {
  private:
	std::string text = "-NO TEXT-";
	Color textColor = {1.0f, 1.0f, 1.0f};
	std::unique_ptr<Font> font;

	Vector2 cachedSize = {};
	float fontSize = -1.0f;

	AlignModeHorizontal alignH = AlignModeHorizontal::CENTER;
	AlignModeVertical alignV = AlignModeVertical::CENTER;

	// bitfield order: font, alignH, text, size, color
	uint8_t isDirty : 2 = 0b11;

	[[maybe_unused]]
	uint64_t _padding1 : 30;

  public:
	UILabel(Font* initialFont);

	bool canDraw() const override;
	bool draw() override;
	bool isValid() const;

	void markDirty();

	void setFont(Font* font);
	void setFontSize(float newSize);
	void setFontAlignHorizontal(AlignModeHorizontal align);
	void setFontAlignVertical(AlignModeVertical align);
	void setText(const std::string& str);
	void setTextColor(const Color& color);

  private:
	bool update();
	SDL_FRect getTextBounds(const SDL_FRect& drawRect) const;
};

}  // namespace Bloodlust
