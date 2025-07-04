#pragma once

#include "uiElement.hpp"

namespace Bloodlust {

class UIRect : public UIElement {
  public:
	bool canDraw() const override;
	bool draw() override;
};

}  // namespace Bloodlust
