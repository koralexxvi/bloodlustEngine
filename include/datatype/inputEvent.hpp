#ifndef BL_DATA_INPUTEVENT_H
#define BL_DATA_INPUTEVENT_H

#include "vector2.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

namespace Bloodlust::DataTypes {

enum class InputState {
	DOWN,
	UP,
	CANCEL
};

enum class InputType {
	KEYBOARD,
	MOUSE_CLICK,
	MOUSE_MOVE,
	MOUSE_SCROLL,
	GAMEPAD_BUTTON,
	GAMEPAD_STICK
};

struct InputEvent {
	const Vector2 position;
	const Vector2 relativePos;
	const InputState state = InputState::DOWN;
	const InputType type;
	const uint32_t device;
	const SDL_Keycode key = 0;
	const int16_t axis = 0;
	const uint8_t input = 0;

  private:
	[[maybe_unused]]
	char _padding[1];

  public:
	InputEvent(const SDL_KeyboardEvent& event);

	InputEvent(const SDL_MouseButtonEvent& event);
	InputEvent(const SDL_MouseMotionEvent& event);
	InputEvent(const SDL_MouseWheelEvent& event);

	InputEvent(const SDL_GamepadButtonEvent& event);
	InputEvent(const SDL_GamepadAxisEvent& event);
};

}  // namespace Bloodlust::DataTypes

#endif
