#pragma once

#include "vector2.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keycode.h>

namespace Bloodlust {

enum class InputState : int32_t {
	DOWN,
	UP,
	CANCEL
};

enum class InputType : int32_t {
	KEYBOARD,
	MOUSE_CLICK,
	MOUSE_MOVE,
	MOUSE_SCROLL,
	GAMEPAD_BUTTON,
	GAMEPAD_AXIS
};

struct InputEvent {
	const Vector2 position;
	const Vector2 relativePos;
	const uint32_t device;
	const InputState state = InputState::DOWN;
	const InputType type;
	const SDL_Keycode key = 0;
	const SDL_GamepadAxis axis = SDL_GAMEPAD_AXIS_INVALID;
	const int32_t input = 0;

  public:
	InputEvent(const SDL_KeyboardEvent& event);

	InputEvent(const SDL_MouseButtonEvent& event);
	InputEvent(const SDL_MouseMotionEvent& event);
	InputEvent(const SDL_MouseWheelEvent& event);

	InputEvent(const SDL_GamepadButtonEvent& event);
	InputEvent(const SDL_GamepadAxisEvent& event);
};

}  // namespace Bloodlust
