#include "../../include/object/uiSprite.hpp"

#include "../../include/graphics.hpp"

using Bloodlust::UISprite;

bool UISprite::canDraw() const {
	// TODO: frustum culling
	// clang-format off
	return visible
		&& (imageColor.a != 0 || backgroundColor.a != 0
			|| (outlineWidth > 0 && outlineColor.a != 0));
	// clang-format on
}

bool UISprite::draw() {
	if (texture == nullptr) {
		return false;
	}

	Vector2 drawPos = getDrawPos();
	Vector2 drawSize = getDrawSize();

	if (backgroundColor.a != 0) {
		Graphics::setDrawColor(backgroundColor);
		if (!Graphics::drawRect(drawPos, drawSize)) {
			return false;
		}
	}

	if (imageColor.a != 0) {
		texture->setColor(imageColor);
		if (!Graphics::drawTexture(texture.get(), drawPos, drawSize)) {
			return false;
		}
	}
	if (outlineWidth > 0 && outlineColor.a != 0) {
		return drawOutline();
	}

	return true;
}
