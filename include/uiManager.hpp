#pragma once

#include "object/uiElement.hpp"

namespace Bloodlust {

bool drawUI();

void registerUIElement(UIElement* element);
void deregisterUIElement(UIElement* element);
bool isUIElementRegistered(UIElement* element);

}  // namespace Bloodlust
