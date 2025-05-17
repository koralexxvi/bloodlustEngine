#ifndef BL_OBJ_UILABEL_H
#define BL_OBJ_UILABEL_H

#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "uiElement.hpp"

#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <string>

namespace Bloodlust::Objects {

class UILabel : UIElement {
  private:
	std::string text = "-NO TEXT-";
	std::unique_ptr<TTF_Text> textObject = nullptr;
	std::unique_ptr<DataTypes::Font> font;

	Vector2 cachedSize = {};
	DataTypes::Color textColor = {255, 255, 255};
	float textScale = 0.9f;

	// bitfield order: font, text, size, color
	uint8_t isDirty : 4 = 0;

	[[maybe_unused]]
	uint64_t _padding : 60;

  public:
	UILabel(DataTypes::Font* initialFont);

	bool canDraw() const override;
	bool draw() override;
	bool isValid() const;

	void markDirty();

	void setFont(DataTypes::Font* font);
	void setText(const std::string& str);
	void setTextScale(float newScale);
	void setTextColor(const DataTypes::Color& color);

  private:
	bool update();
};

}  // namespace Bloodlust::Objects

#endif
