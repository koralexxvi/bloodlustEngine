#include "../../include/object/uiRect.hpp"

#include "../../include/graphics.hpp"

using Bloodlust::UIRect;

bool UIRect::canDraw() const {
	// TODO: frustum culling
	return visible && (backgroundColor.a > 0.0f || (outlineWidth > 0 && outlineColor.a > 0.0f));
}

bool UIRect::draw() {
	Graphics::setDrawColor(backgroundColor);
	Graphics::fillRect(getDrawRect());
	if (outlineWidth > 0 && outlineColor.a > 0.0f) {
		return drawOutline();
	}
	return true;
}
