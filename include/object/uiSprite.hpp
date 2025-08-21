#pragma once

#include "../datatype/texture.hpp"
#include "uiElement.hpp"

#include <memory>

namespace Bloodlust {

class UISprite : public UIElement {
  public:
	Colour imageColor = {1.0f, 1.0f, 1.0f};
	std::unique_ptr<Texture> texture = nullptr;

  public:
	bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const override;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool draw() override;
};

}  // namespace Bloodlust
