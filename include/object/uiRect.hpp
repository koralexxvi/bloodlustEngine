#pragma once

#include "uiElement.hpp"

namespace Bloodlust {

class UIRect : public UIElement {
  public:
	bool canDraw(const Vector2& cliPos, const Vector2& clipSize) const override;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() override;
};

}  // namespace Bloodlust
