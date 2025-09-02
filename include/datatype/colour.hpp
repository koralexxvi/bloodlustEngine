#pragma once

#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <format>
#include <string>

namespace Bloodlust {

// A structure consisting of four floats representing red, green, blue and alpha (transparency).
// Components are expected to be within the range 0.0f-1.0f.
struct Colour {
	// The red value.
	float r;
	// The green value.
	float g;
	// The blue value.
	float b;
	// The alpha value. 0.0f is fully transparent, and 1.0f is fully opaque. Defaults to 1.0f.
	float a = 1.0f;

	// Converts four uint8 values to a Colour struct.
	static constexpr Colour fromInt8(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) {
		return {float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f};
	}

	// Converts a uint32 value to a Colour struct, adjusted for endianness.
	static constexpr Colour fromInt32(std::uint32_t color) {
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

	// Converts this colour to four uint8 values passed by reference.
	void toInt8(std::uint8_t& ir, std::uint8_t& ig, std::uint8_t& ib, std::uint8_t& ia) const {
		ir = std::uint8_t(r * 255.0f);
		ig = std::uint8_t(g * 255.0f);
		ib = std::uint8_t(b * 255.0f);
		ia = std::uint8_t(a * 255.0f);
	}

	// Returns this colour converted to a uint32, adjusted for endianness.
	constexpr std::uint32_t toInt32() const {
		std::uint8_t r, g, b, a;
		toInt8(r, g, b, a);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		return r | (g >> 8) | (b >> 16) | (a >> 24);
#else
		return a | (b >> 8) | (g >> 16) | (r >> 24);
#endif
	}

	// Returns this colour represented as a string.
	constexpr std::string toString() const {
		return std::format("Color({}, {}, {}, {})", r, g, b, a);
	}

	// Converts this colour to an SDL_Color struct.
	constexpr SDL_Color toSDLColor() const {
		std::uint8_t ir, ig, ib, ia;
		toInt8(ir, ig, ib, ia);
		return {ir, ig, ib, ia};
	}

	// Compares this colour to another, returning true if every component is equal.
	constexpr bool operator==(const Colour& other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	// Compares this colour to another, returning false if every component is equal.
	constexpr bool operator!=(const Colour& other) const {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
