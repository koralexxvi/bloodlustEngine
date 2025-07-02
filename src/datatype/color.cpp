#include "../../include/datatype/color.hpp"

#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <format>
#include <string>

using Bloodlust::Color;

void Color::toInt8(uint8_t& ir, uint8_t& ig, uint8_t& ib, uint8_t& ia) const {
	ir = uint8_t(r * 255.0f);
	ig = uint8_t(g * 255.0f);
	ib = uint8_t(b * 255.0f);
	ia = uint8_t(a * 255.0f);
}

std::string Color::toString() const {
	return std::format("Color({}, {}, {}, {})", r, g, b, a);
}

SDL_Color Color::toSDLColor() const {
	uint8_t ir, ig, ib, ia;
	toInt8(ir, ig, ib, ia);
	return {ir, ig, ib, ia};
}

bool Color::operator==(const Color& other) const {
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool Color::operator!=(const Color& other) const {
	return !(*this == other);
}
