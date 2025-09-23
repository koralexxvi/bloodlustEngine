#include "../../include/datatype/inputEvent.hpp"

using Bloodlust::InputEvent;

InputEvent::InputEvent(const SDL_KeyboardEvent& event)
	: device(event.which),
	  type(InputType::KEYBOARD),
	  key(event.key),
	  down(event.down) {}

InputEvent::InputEvent(const SDL_MouseButtonEvent& event)
	: position(event.x, event.y),
	  device(event.which),
	  type(InputType::MOUSE_CLICK),
	  input(event.button),
	  down(event.down) {}

InputEvent::InputEvent(const SDL_MouseMotionEvent& event)
	: position(event.x, event.y),
	  relativePos(event.xrel, event.yrel),
	  device(event.which),
	  type(InputType::MOUSE_MOVE) {}

InputEvent::InputEvent(const SDL_MouseWheelEvent& event)
	: position(event.mouse_x, event.mouse_y),
	  relativePos(event.x, event.y),
	  device(event.which),
	  type(InputType::MOUSE_SCROLL) {}

InputEvent::InputEvent(const SDL_GamepadButtonEvent& event)
	: device(event.which),
	  type(InputType::GAMEPAD_BUTTON),
	  input(event.button),
	  down(event.down) {}

InputEvent::InputEvent(const SDL_GamepadAxisEvent& event)
	: device(event.which),
	  type(InputType::GAMEPAD_AXIS),
	  axis(SDL_GamepadAxis(event.axis)),
	  input(event.value) {}
