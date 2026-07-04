#include "../../include/render/uiManager.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/vector2.hpp"
#include "../../include/object/uiElement.hpp"

#include <unordered_set>

namespace {

std::unordered_set<Bloodlust::UIElement*> elements = {};

}

bool Bloodlust::drawUI() noexcept {
	Vector2 res = app->window->getSize().toVector();
	for (const auto& v : elements) {
		if (v->canDraw({}, res)) {
			if (!v->draw()) {
				return false;
			}
		}
	}
	return true;
}

void Bloodlust::registerUIElement(UIElement* element) noexcept {
	elements.insert(element);
}

void Bloodlust::deregisterUIElement(UIElement* element) noexcept {
	elements.erase(element);
}

bool Bloodlust::isUIElementRegistered(UIElement* element) noexcept {
	return elements.contains(element);
}
