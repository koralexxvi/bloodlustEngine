#include "../../include/object/uiElement.hpp"

#include "../../include/logging.hpp"
#include "../../include/render/uiManager.hpp"

#include <SDL3/SDL_rect.h>
#include <cstdint>

using Bloodlust::UIElement;

bool UIElement::isOutlineVisible() const {
	return outlineWidth != 0.0f && outlineColor.a > 0.0f;
}

bool UIElement::onChildAdded(UIElement*) {
	return true;
}

bool UIElement::onChildRemoved(UIElement*) {
	return true;
}

Bloodlust::UIPoint UIElement::getPosition() const {
	return position;
}

Bloodlust::UIPoint UIElement::getSize() const {
	return size;
}

Bloodlust::Vector2 UIElement::getAnchorPoint() const {
	return anchorPoint;
}

void UIElement::setPosition(const UIPoint& newPos) {
	position = newPos;
	posDirty = true;
}

void UIElement::setSize(const UIPoint& newSize) {
	size = newSize;
	sizeDirty = true;
}

void UIElement::setAnchorPoint(const Vector2& newAnchor) {
	anchorPoint = newAnchor;
	posDirty = true;
}

Bloodlust::Vector2 UIElement::getDrawPos() const {
	if (!posDirty) {
		return cachedPos;
	}
	Vector2 relPos;
	if (parent != nullptr) {
		relPos = parent->getDrawPos() + position.toAbsolutePos(parent->getDrawSize());
	} else {
		relPos = position.toAbsolutePos();
	}
	relPos -= anchorPoint * getDrawSize();
	posDirty = false;
	cachedPos = relPos;
	return relPos;
}

Bloodlust::Vector2 UIElement::getDrawSize() const {
	if (!sizeDirty) {
		return cachedSize;
	}
	Vector2 resolution = getResolution();
	float relativeX = size.xRel;
	float relativeY = size.yRel;
	int32_t xOffset = size.xOffset;
	int32_t yOffset = size.yOffset;
	for (UIElement* p = parent; p != nullptr; p = p->parent) {
		relativeX *= size.xRel;
		relativeY *= size.yRel;
		xOffset += size.xOffset;
		yOffset += size.yOffset;
	}
	Vector2 size = {
		(resolution.x * relativeX) + float(xOffset),
		(resolution.y * relativeY) + float(yOffset)
	};
	sizeDirty = false;
	cachedSize = size;
	return size;
}

SDL_FRect UIElement::getDrawRect() const {
	Vector2 absPos = getDrawPos();
	Vector2 absSize = getDrawSize();
	return {absPos.x, absPos.y, absSize.x, absSize.y};
}

bool UIElement::setParent(UIElement* newParent) {
	if (newParent == this) {
		setError("Cannot parent a UIElement to itself");
		return false;
	}
	if (parent != nullptr) {
		if (newParent == parent) {
			return true;
		}
		parent->children.erase(this);
		if (!parent->onChildRemoved(this)) {
			return false;
		}
	}
	parent = newParent;
	if (parent != nullptr) {
		parent->children.insert(this);
		return parent->onChildAdded(this);
	}
	return true;
}

UIElement::~UIElement() {
	if (isUIElementRegistered(this)) {
		deregisterUIElement(this);
	}
	parent = nullptr;
	children.clear();
}
