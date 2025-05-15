#ifndef BL_OBJ_UILABEL_H
#define BL_OBJ_UILABEL_H

#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "../datatype/texture.hpp"
#include "uiElement.hpp"

#include <SDL3/SDL_surface.h>
#include <memory>
#include <string>

namespace Bloodlust::Objects {

class UILabel : UIElement {
  private:
	std::string text = "-NO TEXT-";
	std::unique_ptr<DataTypes::Texture> cachedText = nullptr;
	DataTypes::Font* font;

	bool isDirty = true;
	Vector2 cachedSize = {};

  public:
	DataTypes::Color textColor;

	bool canDraw() const override;
	bool draw() override;

	void setDirty();
	void setFont(DataTypes::Font* font);
	void setText(const std::string& str);

  private:
	bool update();
};

}  // namespace Bloodlust::Objects

#endif
