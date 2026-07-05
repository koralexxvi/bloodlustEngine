#include <SDL3/SDL_rect.h>
#include <bloodlustEngine/logging.hpp>
#include <bloodlustEngine/object/uiElement.hpp>
#include <bloodlustEngine/render/uiManager.hpp>

using Bloodlust::UIElement;

bool UIElement::isOutlineVisible() const noexcept {
	return outlineWidth != 0.0f && outlineColour.a > 0.0f;
}

bool UIElement::onChildAdded(UIElement*) noexcept {
	return true;
}

bool UIElement::onChildRemoved(UIElement*) noexcept {
	return true;
}

bool UIElement::setParent(UIElement* newParent) noexcept {
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

UIElement::~UIElement() noexcept {
	if (isUIElementRegistered(this)) {
		deregisterUIElement(this);
	}
	parent = nullptr;
	children.clear();
}
