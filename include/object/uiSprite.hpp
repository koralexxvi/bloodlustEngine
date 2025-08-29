#pragma once

#include "../datatype/texture.hpp"
#include "uiElement.hpp"

#include <memory>

namespace Bloodlust {

// A sprite that can be drawn to the screen.
class UISprite : public UIElement {
  public:
	// The colour of the sprite.
	Colour imageColor = {1.0f, 1.0f, 1.0f};

	// A pointer to the texture this sprite will draw.
	std::unique_ptr<Texture> texture = nullptr;

  public:
	// Returns true if this sprite can be drawn.
	bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const override;

	// Draws this sprite and returns true on success.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() override;
};

}  // namespace Bloodlust
