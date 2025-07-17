#include "../../include/object/uiRect.hpp"

#include "../../include/render/graphics.hpp"

using Bloodlust::UIRect;

bool UIRect::canDraw() const {
	// TODO: frustum culling
	return visible && (backgroundColor.a > 0.0f || (outlineWidth > 0 && outlineColor.a > 0.0f));
}

bool UIRect::draw() {
	setDrawColor(backgroundColor);
	fillRect(getDrawRect());
	if (outlineWidth > 0 && outlineColor.a > 0.0f) {
		return drawOutline();
	}
	return true;
}
