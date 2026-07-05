#include <bloodlustEngine/geometry.hpp>
#include <bloodlustEngine/object/uiSprite.hpp>
#include <bloodlustEngine/render/graphics.hpp>

using Bloodlust::UISprite;

bool UISprite::canDraw(const Vector2& clipPos, const Vector2& clipSize) const noexcept {
	if (!visible) {
		return false;
	}
	bool rectVisible = backgroundColour.a > 0.0f || imageColour.a > 0.0f;
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

bool UISprite::draw() noexcept {
	if (texture == nullptr) {
		return false;
	}

	SDL_FRect drawRect = getDrawRect();
	setDrawLayer(layer);
	if (backgroundColour.a > 0.0f) {
		setDrawColour(backgroundColour);
		fillRect(drawRect);
	}
	if (imageColour.a > 0.0f) {
		setDrawColour(imageColour);
		drawTexture(texture, drawRect);
	}
	if (isOutlineVisible()) {
		setDrawColour(outlineColour);
		drawRectOutline(drawRect, outlineWidth);
	}

	return true;
}
