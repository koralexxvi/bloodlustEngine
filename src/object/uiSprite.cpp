#include "../../include/object/uiSprite.hpp"

#include "../../include/graphics.hpp"

using Bloodlust::UISprite;

bool UISprite::canDraw() const {
	// TODO: frustum culling
	return visible
		&& (imageColor.a > 0.0f || backgroundColor.a > 0.0f
			|| (outlineWidth > 0 && outlineColor.a > 0.0f));
}

bool UISprite::draw() {
	if (texture == nullptr) {
		return false;
	}

	Vector2 drawPos = getDrawPos();
	Vector2 drawSize = getDrawSize();

	if (backgroundColor.a > 0.0f) {
		setDrawColor(backgroundColor);
		drawRect(drawPos, drawSize);
	}

	if (imageColor.a > 0.0f) {
		texture->setColor(imageColor);
		drawTexture(texture.get(), drawPos, drawSize);
	}
	if (outlineWidth > 0 && outlineColor.a > 0.0f) {
		return drawOutline();
	}

	return true;
}
