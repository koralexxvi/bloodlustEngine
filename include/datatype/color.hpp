#pragma once

#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <format>
#include <string>

namespace Bloodlust {

struct Color {
	float r;
	float g;
	float b;
	float a = 1.0f;

	void toInt8(uint8_t& ir, uint8_t& ig, uint8_t& ib, uint8_t& ia) const {
		ir = uint8_t(r * 255.0f);
		ig = uint8_t(g * 255.0f);
		ib = uint8_t(b * 255.0f);
		ia = uint8_t(a * 255.0f);
	}

	constexpr std::string toString() const {
		return std::format("Color({}, {}, {}, {})", r, g, b, a);
	}

	constexpr SDL_Color toSDLColor() const {
		uint8_t ir, ig, ib, ia;
		toInt8(ir, ig, ib, ia);
		return {ir, ig, ib, ia};
	}

	constexpr bool operator==(const Color& other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	constexpr bool operator!=(const Color& other) const {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
