#ifndef BL_OBJ_UIRECT_H
#define BL_OBJ_UIRECT_H

#include "uiElement.hpp"

namespace Bloodlust::Objects {

class UIRect : public UIElement {
  public:
	bool canDraw() const override;
	bool draw() override;
};

}  // namespace Bloodlust::Objects

#endif
