#pragma once

#include "uiElement.hpp"

namespace Bloodlust {

class UIRect : public UIElement {
  public:
	bool canDraw() const override;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() override;
};

}  // namespace Bloodlust
