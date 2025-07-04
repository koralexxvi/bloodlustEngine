#ifndef BL_OBJ_UIMANAGER_H
#define BL_OBJ_UIMANAGER_H

#include "object/uiElement.hpp"

namespace Bloodlust {

bool drawUI();

void registerUIElement(UIElement* element);
void deregisterUIElement(UIElement* element);
bool isUIElementRegistered(UIElement* element);

}  // namespace Bloodlust

#endif
