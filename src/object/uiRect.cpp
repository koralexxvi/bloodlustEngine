#include "../../include/object/uiRect.hpp"

#include "../../include/geometry.hpp"
#include "../../include/render/graphics.hpp"

#include <SDL3/SDL_rect.h>

using Bloodlust::UIRect;

bool UIRect::canDraw(const Vector2& clipPos, const Vector2& clipSize) const {
	if (!visible) {
		return false;
	}
	bool rectVisible = backgroundColour.a > 0.0f;
	bool outlineVisible = isOutlineVisible();
	if (!(rectVisible || outlineVisible)) {
		return false;
	}
	SDL_FRect drawRect = getDrawRect();
	if (outlineVisible && outlineWidth > 0.0f) {
		float doubleWidth = outlineWidth * 2.0f;
		drawRect.x -= outlineWidth;
		drawRect.y -= outlineWidth;
		drawRect.w += doubleWidth;
		drawRect.h += doubleWidth;
	}
	return rectIntersectsRect(
		{drawRect.x, drawRect.y},
		{drawRect.w, drawRect.h},
		clipPos,
		clipSize
	);
}

bool UIRect::draw() {
	SDL_FRect drawRect = getDrawRect();
	setDrawColour(backgroundColour);
	setDrawLayer(layer);
	fillRect(drawRect);
	if (isOutlineVisible()) {
		setDrawColour(outlineColour);
		drawRectOutline(drawRect, outlineWidth);
	}
	return true;
}
