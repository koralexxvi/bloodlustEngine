#include "../../include/object/uiElement.hpp"

#include "../../include/logging.hpp"
#include "../../include/render/uiManager.hpp"

#include <SDL3/SDL_rect.h>
#include <cstdint>

using Bloodlust::UIElement;

bool UIElement::onChildAdded(UIElement* child) {
	return true;
}

bool UIElement::onChildRemoved(UIElement* child) {
	return true;
}

// TODO: cache the draw pos/size
Bloodlust::Vector2 UIElement::getDrawPos() const {
	Vector2 relPos;
	if (parent != nullptr) {
		relPos = parent->getDrawPos() + position.toAbsolutePos(parent->getDrawSize());
	} else {
		relPos = position.toAbsolutePos();
	}
	relPos -= anchorPoint * getDrawSize();
	return relPos;
}

// TODO: cache the draw pos/size
Bloodlust::Vector2 UIElement::getDrawSize() const {
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
	return {
		(resolution.x * relativeX) + float(xOffset),
		(resolution.y * relativeY) + float(yOffset)
	};
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
