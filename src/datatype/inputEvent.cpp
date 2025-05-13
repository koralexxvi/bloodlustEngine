#include "../../include/datatype/inputEvent.hpp"

using Bloodlust::DataTypes::InputEvent, Bloodlust::DataTypes::InputState;

static InputState inputStateFromBool(bool state) {
	return state ? InputState::DOWN : InputState::UP;
}

InputEvent::InputEvent(const SDL_KeyboardEvent& event)
	: state(inputStateFromBool(event.down)),
	  type(InputType::KEYBOARD),
	  device(event.which),
	  key(event.key) {}

InputEvent::InputEvent(const SDL_MouseButtonEvent& event)
	: position(event.x, event.y),
	  state(inputStateFromBool(event.down)),
	  type(InputType::MOUSE_CLICK),
	  device(event.which),
	  input(event.button) {}

InputEvent::InputEvent(const SDL_MouseMotionEvent& event)
	: position(event.x, event.y),
	  relativePos(event.xrel, event.yrel),
	  type(InputType::MOUSE_MOVE),
	  device(event.which) {}

InputEvent::InputEvent(const SDL_MouseWheelEvent& event)
	: position(event.mouse_x, event.mouse_y),
	  relativePos(event.x, event.y),
	  type(InputType::MOUSE_SCROLL),
	  device(event.which) {}

InputEvent::InputEvent(const SDL_GamepadButtonEvent& event)
	: state(inputStateFromBool(event.down)),
	  type(InputType::GAMEPAD_BUTTON),
	  device(event.which),
	  input(event.button) {}

InputEvent::InputEvent(const SDL_GamepadAxisEvent& event)
	: type(InputType::GAMEPAD_STICK),
	  device(event.which),
	  axis(event.value),
	  input(event.axis) {}
