#pragma once

#include "uiElement.hpp"

namespace Bloodlust {

// A rectangle that can be drawn to the screen.
class UIRect : public UIElement {
  public:
	// Returns true if this rect can be drawn.
	bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const noexcept override;

	// Draws this rect. Will throw InternalError on failure.
	void draw() override;
};

}  // namespace Bloodlust
