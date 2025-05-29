#include "../../include/object/uiRect.hpp"

#include "../../include/graphics.hpp"

#include <SDL3/SDL_rect.h>

using Bloodlust::UIRect;

bool UIRect::canDraw() const {
	// TODO: frustum culling
	return visible && (backgroundColor.a != 0 || (outlineWidth > 0 && outlineColor.a != 0));
}

bool UIRect::draw() {
	Graphics::setDrawColor(backgroundColor);
	SDL_FRect rect = getDrawRect();
	if (!Graphics::fillRect(&rect)) {
		return false;
	}
	if (outlineWidth > 0 && outlineColor.a != 0) {
		return drawOutline();
	}
	return true;
}
