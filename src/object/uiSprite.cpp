#include "../../include/object/uiSprite.hpp"

#include "../../include/graphics.hpp"

using Bloodlust::Objects::UISprite;

bool UISprite::canDraw() const {
	// TODO: frustum culling
	return visible && (imageColor.a != 0 || backgroundColor.a != 0);
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

	return true;
}
