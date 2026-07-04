#pragma once

#include "../util.hpp"

namespace Bloodlust {

class UIElement;

// Queues each registered UI element for drawing, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool drawUI() noexcept;

// Registers the given element to be drawn during the render step.
void registerUIElement(UIElement* element) noexcept;

// Deregisters the given element from being drawn during the render step.
void deregisterUIElement(UIElement* element) noexcept;

// Returns true if this element is currently registered to be drawn during the render step.
bool isUIElementRegistered(UIElement* element) noexcept;

}  // namespace Bloodlust
