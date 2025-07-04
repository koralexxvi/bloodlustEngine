#pragma once

#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <string>

namespace Bloodlust {

struct Color {
	float r;
	float g;
	float b;
	float a = 1.0f;

	void toInt8(uint8_t& fr, uint8_t& fg, uint8_t& fb, uint8_t& fa) const;
	std::string toString() const;
	SDL_Color toSDLColor() const;

	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;
};

}  // namespace Bloodlust
