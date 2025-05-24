#ifndef BL_OBJ_UILABEL_H
#define BL_OBJ_UILABEL_H

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
	std::unique_ptr<Font> font;
	TTF_Text* textObject = nullptr;

	Vector2 cachedSize = {};
	Vector2 textSize = {};
	Color textColor = {255, 255, 255};
	float fontSize = -1.0f;

	AlignModeHorizontal alignH = AlignModeHorizontal::CENTER;
	AlignModeVertical alignV = AlignModeVertical::CENTER;

	// bitfield order: font, alignH, text, size, color
	uint8_t isDirty : 5 = 0b11111;

	[[maybe_unused]]
	uint64_t _padding1 : 27;
	[[maybe_unused]]
	char _padding2[4];

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

	~UILabel();

  private:
	bool update();
	SDL_FRect getTextBounds(const SDL_FRect& drawRect) const;
};

}  // namespace Bloodlust

#endif
