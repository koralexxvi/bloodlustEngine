#pragma once

#include "../object/uiElement.hpp"
#include "../util.hpp"

namespace Bloodlust {

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool drawUI();

void registerUIElement(UIElement* element);
void deregisterUIElement(UIElement* element);
bool isUIElementRegistered(UIElement* element);

}  // namespace Bloodlust
