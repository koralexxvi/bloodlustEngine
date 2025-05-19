#ifndef BL_OBJ_UIMANAGER_H
#define BL_OBJ_UIMANAGER_H

#include "object/uiElement.hpp"

namespace Bloodlust::UIManager {

bool draw();

void registerElement(UIElement* element);
void deregisterElement(UIElement* element);
bool isElementRegistered(UIElement* element);

}  // namespace Bloodlust::UIManager

#endif
