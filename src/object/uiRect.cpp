#include "../../include/object/uiRect.hpp"

#include "../../include/render/graphics.hpp"

using Bloodlust::UIRect;

bool UIRect::canDraw() const {
	// TODO: frustum culling
	return visible && (backgroundColor.a > 0.0f || isOutlineVisible());
}

bool UIRect::draw() {
	SDL_FRect drawRect = getDrawRect();
	setDrawColor(backgroundColor);
	fillRect(drawRect);
	if (isOutlineVisible()) {
		setDrawColor(outlineColor);
		drawRectOutline(drawRect, outlineWidth);
	}
	return true;
}
