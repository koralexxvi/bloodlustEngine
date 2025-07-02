#ifndef BL_OBJ_UISPRITE_H
#define BL_OBJ_UISPRITE_H

#include "../datatype/texture.hpp"
#include "uiElement.hpp"

#include <memory>

namespace Bloodlust {

class UISprite : public UIElement {
  public:
	Color imageColor = {1.0f, 1.0f, 1.0f};
	std::unique_ptr<Texture> texture = nullptr;

  public:
	bool canDraw() const override;
	bool draw() override;
};

}  // namespace Bloodlust

#endif
