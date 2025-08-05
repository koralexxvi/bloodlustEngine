#include "../../include/object/uiSprite.hpp"

#include "../../include/render/graphics.hpp"

using Bloodlust::UISprite;

bool UISprite::canDraw() const {
	// TODO: frustum culling
	return visible && (imageColor.a > 0.0f || backgroundColor.a > 0.0f || isOutlineVisible());
}

bool UISprite::draw() {
	if (texture == nullptr) {
		return false;
	}

	SDL_FRect drawRect = getDrawRect();
	if (backgroundColor.a > 0.0f) {
		setDrawColor(backgroundColor);
		fillRect(drawRect, zIndex);
	}
	if (imageColor.a > 0.0f) {
		setDrawColor(imageColor);
		drawTexture(texture.get(), drawRect);
	}
	if (isOutlineVisible()) {
		setDrawColor(outlineColor);
		drawRectOutline(drawRect, outlineWidth, zIndex);
	}

	return true;
}
