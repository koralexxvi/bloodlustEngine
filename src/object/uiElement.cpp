#include "../../include/object/uiElement.hpp"

#include "../../include/uiManager.hpp"

#include <SDL3/SDL_rect.h>

using Bloodlust::DataTypes::Vector2;
using Bloodlust::Objects::UIElement;

UIElement::UIElement() {}

Vector2 UIElement::getDrawPos() const {
	Vector2 relPos = position.toAbsolutePos();
	if (parent != nullptr) {
		relPos += parent->getDrawPos();
	}
	relPos += anchorPoint * getDrawSize();
	return relPos;
}

Vector2 UIElement::getDrawSize() const {
	return parent != nullptr ? size.toAbsolutePos(parent->getDrawSize()) : size.toAbsolutePos();
}

SDL_FRect UIElement::getDrawRect() const {
	Vector2 absPos = position.toAbsolutePos();
	Vector2 absSize = size.toAbsolutePos();
	if (parent != nullptr) {
		absPos += parent->getDrawPos();
		absSize *= parent->getDrawSize();
	}
	absPos += absSize * anchorPoint;
	return {absPos.x, absPos.y, absSize.x, absSize.y};
}

bool UIElement::onChildAdded(UIElement* child) {
	return true;
}

bool UIElement::onChildRemoved(UIElement* child) {
	return true;
}

bool UIElement::setParent(UIElement* newParent) {
	if (parent != nullptr) {
		if (newParent == parent) {
			return true;
		}
		parent->children.erase(this);
		if (!parent->onChildRemoved(this)) {
			return false;
		}
	}
	if (parent != nullptr && parent->parent == nullptr) {
		delete parent;
	}
	parent = newParent;
	if (parent != nullptr) {
		parent->children.insert(this);
		return parent->onChildAdded(this);
	}
	return true;
}

UIElement::~UIElement() {
	UIManager::deregisterElement(this);
	parent = nullptr;
	for (auto&& v : children) {
		delete v;
	}
	children.clear();
}
