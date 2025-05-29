#include "../include/uiManager.hpp"

#include <cstdint>
#include <unordered_set>

using namespace Bloodlust;

namespace {

std::unordered_set<UIElement*> elements[UINT8_MAX] = {};

}

bool UIManager::draw() {
	for (const auto& v : elements) {
		for (const auto& v2 : v) {
			if (!v2->draw()) {
				return false;
			}
		}
	}
	return true;
}

void UIManager::registerElement(UIElement* element) {
	elements[element->getZIndex()].insert(element);
}

void UIManager::deregisterElement(UIElement* element) {
	elements[element->getZIndex()].erase(element);
}

bool UIManager::isElementRegistered(UIElement* element) {
	return elements[element->getZIndex()].contains(element);
}
