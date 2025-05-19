#ifndef BL_OBJ_UISPRITE_H
#define BL_OBJ_UISPRITE_H

#include "../datatype/texture.hpp"
#include "uiElement.hpp"

#include <memory>

namespace Bloodlust {

class UISprite : public UIElement {
  public:
	std::unique_ptr<Texture> texture = nullptr;
	Color imageColor = {255, 255, 255, 255};

  private:
	[[maybe_unused]]
	char _padding[4];

  public:
	bool canDraw() const override;
	bool draw() override;
};

}  // namespace Bloodlust

#endif
