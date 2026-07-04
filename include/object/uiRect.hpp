#pragma once

#include "uiElement.hpp"

namespace Bloodlust {

// A rectangle that can be drawn to the screen.
class UIRect : public UIElement {
  public:
	// Returns true if this rect can be drawn.
	bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const noexcept override;

	// Draws this rect and returns true on success.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() noexcept override;
};

}  // namespace Bloodlust
