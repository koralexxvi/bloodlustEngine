#include "../../include/object/uiElement.hpp"

#include "../../include/graphics.hpp"
#include "../../include/logging.hpp"
#include "../../include/uiManager.hpp"

#include <SDL3/SDL_rect.h>
#include <cstdint>

using Bloodlust::UIElement;
using Bloodlust::Vector2;

// TODO: cache the draw pos/size
Vector2 UIElement::getDrawPos() const {
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
Vector2 UIElement::getDrawSize() const {
	Vector2 resolution = Video::getResolution();
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

uint8_t UIElement::getZIndex() const {
	return zIndex;
}

void UIElement::setZIndex(uint8_t index) {
	bool isRegistered = UIManager::isElementRegistered(this);
	if (isRegistered) {
		UIManager::deregisterElement(this);
	}
	zIndex = index;
	if (isRegistered) {
		UIManager::registerElement(this);
	}
}

bool UIElement::setParent(UIElement* newParent) {
	if (newParent == this) {
		Logging::setError("Cannot parent a UIElement to itself");
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

bool UIElement::drawOutline() const {
	Graphics::setDrawColor(outlineColor);
	SDL_FRect rect = getDrawRect();
	bool success = true;
	for (int i = 0; i < outlineWidth; i++) {
		rect.x--;
		rect.y--;
		rect.w += 2.0f;
		rect.h += 2.0f;
		success &= Graphics::drawRect(&rect);
	}
	return success;
}

bool UIElement::onChildAdded(UIElement* child) {
	return true;
}

bool UIElement::onChildRemoved(UIElement* child) {
	return true;
}

UIElement::~UIElement() {
	if (UIManager::isElementRegistered(this)) {
		UIManager::deregisterElement(this);
	}
	parent = nullptr;
	children.clear();
}
