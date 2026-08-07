#include <SDL3/SDL_rect.h>
#include <bloodlustEngine/logging.hpp>
#include <bloodlustEngine/object/uiElement.hpp>
#include <bloodlustEngine/render/uiManager.hpp>

using Bloodlust::UIElement;

UIElement::~UIElement() noexcept {
	if (isUIElementRegistered(this)) {
		deregisterUIElement(this);
	}
	parent = nullptr;
	children.clear();
}

bool UIElement::isOutlineVisible() const noexcept {
	return outlineWidth != 0.0f && outlineColour.a > 0.0f;
}

void UIElement::onChildAdded(UIElement*) noexcept {}

void UIElement::onChildRemoved(UIElement*) noexcept {}

void UIElement::setParent(UIElement* newParent) {
	if (newParent == this) {
		log(SDL_LOG_PRIORITY_WARN, "WARNING: Attempt to parent UIElement to itself");
		return;
	}
	if (parent != nullptr) {
		if (newParent == parent) {
			return;
		}
		parent->children.erase(this);
		parent->onChildRemoved(this);
	}
	parent = newParent;
	if (parent != nullptr) {
		parent->children.insert(this);
		parent->onChildAdded(this);
	}
}
