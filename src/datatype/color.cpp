#include "../../include/datatype/color.hpp"

#include <SDL3/SDL_pixels.h>

using Bloodlust::Color;

void Color::toFloat(float* fr, float* fg, float* fb, float* fa) const {
	if (fr != nullptr) {
		*fr = r / 255.0f;
	}
	if (fg != nullptr) {
		*fg = g / 255.0f;
	}
	if (fb != nullptr) {
		*fb = b / 255.0f;
	}
	if (fa != nullptr) {
		*fa = a / 255.0f;
	}
}

SDL_Color Color::toSDLColor() const {
	return {r, g, b, a};
}

bool Color::operator==(const Color& other) const {
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool Color::operator!=(const Color& other) const {
	return !(*this == other);
}
