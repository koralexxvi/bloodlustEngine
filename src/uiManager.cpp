#include "../include/uiManager.hpp"

#include <cstdint>
#include <unordered_set>

using namespace Bloodlust;

static std::unordered_set<UIElement*> elements[UINT8_MAX] = {};

bool UIManager::draw() {
	for (auto&& v : elements) {
		for (auto&& v2 : v) {
			if (!v2->draw()) {
				return false;
			}
		}
	}
	return true;
}

void UIManager::registerElement(UIElement* element) {
	elements[element->zIndex].insert(element);
}

void UIManager::deregisterElement(UIElement* element) {
	elements[element->zIndex].erase(element);
}

bool UIManager::isElementRegistered(UIElement* element) {
	return elements[element->zIndex].contains(element);
}
