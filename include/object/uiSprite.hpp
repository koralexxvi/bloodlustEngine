#ifndef BL_OBJ_UISPRITE_H
#define BL_OBJ_UISPRITE_H

#include "../datatype/texture.hpp"
#include "uiElement.hpp"

#include <memory>

namespace Bloodlust::Objects {

class UISprite : UIElement {
  public:
	std::unique_ptr<DataTypes::Texture> texture = nullptr;
	DataTypes::Color imageColor = {255, 255, 255, 255};

  private:
	char _padding[4];

  public:
	bool canDraw() const override;
	bool draw() override;
};

}  // namespace Bloodlust::Objects

#endif
