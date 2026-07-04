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
	static constexpr Colour fromInt8(
		std::uint8_t r,
		std::uint8_t g,
		std::uint8_t b,
		std::uint8_t a = 255
	) noexcept {
		return {
			static_cast<float>(r) / 255.0f,
			static_cast<float>(g) / 255.0f,
			static_cast<float>(b) / 255.0f,
			static_cast<float>(a) / 255.0f
		};
	}

	// Converts a uint32 value to a Colour struct, adjusted for endianness.
	static constexpr Colour fromInt32(std::uint32_t colour) noexcept {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		return fromInt8(
			colour & 0xFF,
			(colour >> 8) & 0xFF,
			(colour >> 16) & 0xFF,
			(colour >> 24) & 0xFF
		);
#else
		return fromInt8(
			(colour >> 24) & 0xFF,
			(colour >> 16) & 0xFF,
			(colour >> 8) & 0xFF,
			colour & 0xFF
		);
#endif
	}

	// Converts this colour to four uint8 values passed by reference.
	void toInt8(
		std::uint8_t& ir,
		std::uint8_t& ig,
		std::uint8_t& ib,
		std::uint8_t& ia
	) const noexcept {
		ir = static_cast<std::uint8_t>(r * 255.0f);
		ig = static_cast<std::uint8_t>(g * 255.0f);
		ib = static_cast<std::uint8_t>(b * 255.0f);
		ia = static_cast<std::uint8_t>(a * 255.0f);
	}

	// Returns this colour converted to a uint32, adjusted for endianness.
	constexpr std::uint32_t toInt32() const noexcept {
		std::uint8_t ir = static_cast<std::uint8_t>(r * 255.0f);
		std::uint8_t ig = static_cast<std::uint8_t>(g * 255.0f);
		std::uint8_t ib = static_cast<std::uint8_t>(b * 255.0f);
		std::uint8_t ia = static_cast<std::uint8_t>(a * 255.0f);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		return ir | (ig >> 8) | (ib >> 16) | (ia >> 24);
#else
		return ia | (ib >> 8) | (ig >> 16) | (ir >> 24);
#endif
	}

	// Returns this colour represented as a string.
	inline std::string toString() const noexcept {
		return std::format("Colour({}, {}, {}, {})", r, g, b, a);
	}

	// Converts this colour to an SDL_Color struct.
	SDL_Color toSDLColour() const noexcept {
		std::uint8_t ir, ig, ib, ia;
		toInt8(ir, ig, ib, ia);
		return {ir, ig, ib, ia};
	}

	// Compares this colour to another, returning true if every component is equal.
	constexpr bool operator==(const Colour& other) const noexcept {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	// Compares this colour to another, returning false if every component is equal.
	constexpr bool operator!=(const Colour& other) const noexcept {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
