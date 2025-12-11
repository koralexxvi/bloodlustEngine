#include "../../include/render/uiManager.hpp"

#include "../../include/app.hpp"

#include <unordered_set>

namespace {

std::unordered_set<Bloodlust::UIElement*> elements = {};

}

bool Bloodlust::drawUI() {
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

void Bloodlust::registerUIElement(UIElement* element) {
	elements.insert(element);
}

void Bloodlust::deregisterUIElement(UIElement* element) {
	elements.erase(element);
}

bool Bloodlust::isUIElementRegistered(UIElement* element) {
	return elements.contains(element);
}
