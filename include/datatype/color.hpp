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

	static constexpr Color fromInt8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
		return {float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f};
	}

	static constexpr Color fromInt32(uint32_t color) {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		return fromInt8(
			color & 0xFF,
			(color >> 8) & 0xFF,
			(color >> 16) & 0xFF,
			(color >> 24) & 0xFF
		);
#else
		return fromInt8(
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >> 8) & 0xFF,
			color & 0xFF
		);
#endif
	}

	void toInt8(uint8_t& ir, uint8_t& ig, uint8_t& ib, uint8_t& ia) const {
		ir = uint8_t(r * 255.0f);
		ig = uint8_t(g * 255.0f);
		ib = uint8_t(b * 255.0f);
		ia = uint8_t(a * 255.0f);
	}

	constexpr uint32_t toInt32() const {
		uint8_t r, g, b, a;
		toInt8(r, g, b, a);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		return r | (g >> 8) | (b >> 16) | (a >> 24);
#else
		return a | (b >> 8) | (g >> 16) | (r >> 24);
#endif
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
