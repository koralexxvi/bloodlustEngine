#include "../../include/object/uiSprite.hpp"

#include "../../include/render/graphics.hpp"

using Bloodlust::UISprite;

bool UISprite::canDraw() const {
	// TODO: frustum culling
	return visible
		&& (imageColor.a > 0.0f || backgroundColor.a > 0.0f
			|| (outlineWidth != 0.0f && outlineColor.a > 0.0f));
}

bool UISprite::draw() {
	if (texture == nullptr) {
		return false;
	}

	SDL_FRect drawRect = getDrawRect();
	if (backgroundColor.a > 0.0f) {
		setDrawColor(backgroundColor);
		fillRect(drawRect);
	}
	if (imageColor.a > 0.0f) {
		texture->color = imageColor;
		drawTexture(texture.get(), drawRect);
	}
	if (outlineWidth != 0.0f && outlineColor.a > 0.0f) {
		setDrawColor(outlineColor);
		drawRectOutline(drawRect, outlineWidth);
	}

	return true;
}
