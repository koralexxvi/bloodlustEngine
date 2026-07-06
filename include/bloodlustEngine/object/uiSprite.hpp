#pragma once

#include "uiElement.hpp"

namespace Bloodlust {

struct Colour;
struct Vector2;
class Texture;

// A sprite that can be drawn to the screen.
class UISprite : public UIElement {
  public:
	// The colour of the sprite.
	Colour imageColour = {1.0f, 1.0f, 1.0f};

	// A pointer to the texture this sprite will draw.
	Texture* texture = nullptr;

  public:
	// Returns true if this sprite can be drawn.
	bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const noexcept override;

	// Draws this sprite. Will throw InternalError on failure.
	void draw() override;
};

}  // namespace Bloodlust
