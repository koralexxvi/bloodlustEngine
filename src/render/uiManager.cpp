#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/datatype/vector2.hpp>
#include <bloodlustEngine/object/uiElement.hpp>
#include <bloodlustEngine/render/uiManager.hpp>
#include <unordered_set>

namespace {

std::unordered_set<Bloodlust::UIElement*> elements = {};

}

void Bloodlust::drawUI() {
	Vector2 res = app->window->getSize().toVector();
	for (const auto& v : elements) {
		if (v->canDraw({}, res)) {
			v->draw();
		}
	}
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
