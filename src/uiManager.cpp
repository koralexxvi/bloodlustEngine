#include "../include/uiManager.hpp"

#include <cstdint>
#include <unordered_set>

namespace {

std::unordered_set<Bloodlust::UIElement*> elements[UINT8_MAX] = {};

}

bool Bloodlust::drawUI() {
	for (const auto& v : elements) {
		for (const auto& v2 : v) {
			if (!v2->draw()) {
				return false;
			}
		}
	}
	return true;
}

void Bloodlust::registerUIElement(UIElement* element) {
	elements[element->getZIndex()].insert(element);
}

void Bloodlust::deregisterUIElement(UIElement* element) {
	elements[element->getZIndex()].erase(element);
}

bool Bloodlust::isUIElementRegistered(UIElement* element) {
	return elements[element->getZIndex()].contains(element);
}
