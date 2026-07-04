#pragma once

#include "vector2.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keycode.h>
#include <cstdint>

namespace Bloodlust {

// Represents an input type.
enum class InputType : std::int32_t {
	KEYBOARD,		 // A keypress was processed.
	MOUSE_CLICK,	 // A mouse was clicked.
	MOUSE_MOVE,		 // A mouse was moved.
	MOUSE_SCROLL,	 // A mouse wheel was scrolled.
	GAMEPAD_BUTTON,	 // A gamepad button was pressed or released.
	GAMEPAD_AXIS	 // An axis on a gamepad was updated. This can be either an axis on an analogue
					 // stick or an analogue trigger.
};

// A structure that represents an input event.
struct InputEvent {
	// The position of the input. Used for mouse movement events.
	const Vector2 position;

	// The relative position of the input. Used for mouse movement events.
	const Vector2 relativePos;

	// The device ID this event was produced by.
	const std::uint32_t device;

	// The type of input that this event represents.
	const InputType type;

	// The keycode of which key was pressed. Used for keyboard events.
	const SDL_Keycode key = 0;

	// The gamepad axis that was updated. Used for gamepad axis events.
	const SDL_GamepadAxis axis = SDL_GAMEPAD_AXIS_INVALID;

	// The input value associated with this event. Used for mouse and gamepad button events.
	const std::int32_t input = 0;

	// Whether or not this input was pressed or released. Used for keypress, mouse click and
	// gamepad button inputs.
	const bool down = false;

	// Constructs an InputEvent from a keyboard event.
	InputEvent(const SDL_KeyboardEvent& event) noexcept;

	// Constructs an InputEvent from a mouse button input.
	InputEvent(const SDL_MouseButtonEvent& event) noexcept;

	// Constructs an InputEvent from a mouse motion event.
	InputEvent(const SDL_MouseMotionEvent& event) noexcept;

	// Constructs an InputEvent from a mouse wheel event.
	InputEvent(const SDL_MouseWheelEvent& event) noexcept;

	// Constructs an InputEvent from a gamepad button input.
	InputEvent(const SDL_GamepadButtonEvent& event) noexcept;

	// Constructs an InputEvent from a gamepad axis update.
	InputEvent(const SDL_GamepadAxisEvent& event) noexcept;
};

}  // namespace Bloodlust
