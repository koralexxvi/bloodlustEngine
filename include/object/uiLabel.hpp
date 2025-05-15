#ifndef BL_OBJ_UILABEL_H
#define BL_OBJ_UILABEL_H

#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "uiElement.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <string>

namespace Bloodlust::Objects {

class UILabel : UIElement {
  private:
	DataTypes::Font* font;

  public:
	DataTypes::Color textColor;

	bool canDraw() const override;
	bool draw() override;

	void setFont(DataTypes::Font* font);
	void setText(const std::string& str);
};

}  // namespace Bloodlust::Objects

#endif
