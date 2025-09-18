#include "../../include/object/uiElement.hpp"

#include "../../include/logging.hpp"
#include "../../include/render/uiManager.hpp"

#include <SDL3/SDL_rect.h>

using Bloodlust::UIElement;

bool UIElement::isOutlineVisible() const {
	return outlineWidth != 0.0f && outlineColour.a > 0.0f;
}

bool UIElement::onChildAdded(UIElement*) {
	return true;
}

bool UIElement::onChildRemoved(UIElement*) {
	return true;
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
