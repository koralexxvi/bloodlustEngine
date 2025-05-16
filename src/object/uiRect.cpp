#include "../../include/object/uiRect.hpp"

#include "../../include/graphics.hpp"
#include <SDL3/SDL_rect.h>

using Bloodlust::Objects::UIRect;

bool UIRect::canDraw() const {
	// TODO: frustum culling
	return visible && backgroundColor.a != 0;
}

bool UIRect::draw() {
	Graphics::setDrawColor(backgroundColor);
	SDL_FRect rect = getDrawRect();
	return Graphics::fillRect(&rect);
}
