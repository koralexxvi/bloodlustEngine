#pragma once

#include "../util.hpp"
#include "vector2.hpp"

#include <format>
#include <limits>
#include <string>
#include <type_traits>

namespace Bloodlust {

// A point composed of two integral components.
template<typename T>
	requires(std::is_integral<T>::value)
struct Point {
	// The function object that provides hashing functions for the given point.
	struct Hasher {
		// The hashing function for the point.
		constexpr std::size_t operator()(const Point<T>& p) const noexcept {
			constexpr std::size_t bits = std::numeric_limits<T>::digits;
			static_assert(
				bits == 8 || bits == 16 || bits == 32 || bits == 64,
				"hashing function is only valid for types of 8, 16, 32 or 64 bits"
			);
			if constexpr (bits == 8) {
				return static_cast<std::size_t>(p.x) | (static_cast<std::size_t>(p.y << 8));
			} else if constexpr (bits == 16) {
				return static_cast<std::size_t>(p.x) | (static_cast<std::size_t>(p.y << 16));
			} else if constexpr (bits == 32) {
				return static_cast<std::size_t>(p.x) | (static_cast<std::size_t>(p.y << 32));
			} else {
				std::size_t seed = 0;
				combineHashes(seed, p.x, p.y);
				return seed;
			}
		}
	};

	// The X component of this point.
	T x;

	// The Y component of this point.
	T y;

	// Constructs a vector with an X and Y value of 0.
	constexpr Point() noexcept : x(0), y(0) {}

	// Constructs a vector with the provided components.
	constexpr Point(T x, T y) noexcept : x(x), y(y) {}

	// Converts this point to a string.
	constexpr std::string toString() const noexcept {
		return std::format("<{}, {}>", x, y);
	}

	// Converts this point to a vector.
	constexpr Vector2 toVector() const noexcept {
		return {static_cast<float>(x), static_cast<float>(y)};
	}

	// Returns this point.
	constexpr Point<T> operator+() const noexcept {
		return *this;
	}

	// Returns the unary negation of this point.
	constexpr Point<T> operator-() const noexcept {
		return {-x, -y};
	}

	// Returns the bitwise negation of this point.
	constexpr Point<T> operator~() const noexcept {
		return {~x, ~y};
	}

	// Adds the components of the two points together.
	constexpr Point<T> operator+(const Point<T>& other) const noexcept {
		return {x + other.x, y + other.y};
	}

	// Subtracts the components of this point from the other.
	constexpr Point<T> operator-(const Point<T>& other) const noexcept {
		return {x - other.x, y - other.y};
	}

	// Multiplies the components of this point by a scalar.
	constexpr Point<T> operator*(T scale) const noexcept {
		return {x * scale, y * scale};
	}

	// Multiplies the components of the two points together.
	constexpr Point<T> operator*(const Point<T>& other) const noexcept {
		return {x * other.x, y * other.y};
	}

	// Divides the components of this point by a scalar.
	constexpr Point<T> operator/(T scale) const noexcept {
		return {x / scale, y / scale};
	}

	// Divides the components of this point by the other.
	constexpr Point<T> operator/(const Point<T>& other) const noexcept {
		return {x / other.x, y / other.y};
	}

	// Returns the bitwise and of the two points together.
	constexpr Point<T> operator&(const Point<T>& other) const noexcept {
		return {x & other.x, y & other.y};
	}

	// Returns the bitwise or of the two points together.
	constexpr Point<T> operator|(const Point<T>& other) const noexcept {
		return {x | other.x, y | other.y};
	}

	// Returns this point shifted left by scale bits.
	constexpr Point<T> operator<<(T scale) const noexcept {
		return {x << scale, y << scale};
	}

	// Returns this point's components shifted left by eachother.
	constexpr Point<T> operator<<(const Point<T>& other) const noexcept {
		return {x << other.x, y << other.y};
	}

	// Returns this point shifted right by scale bits.
	constexpr Point<T> operator>>(T scale) const noexcept {
		return {x >> scale, y >> scale};
	}

	// Returns this point's components shifted right by eachother.
	constexpr Point<T> operator>>(const Point<T>& other) const noexcept {
		return {x >> other.x, y >> other.y};
	}

	// Adds the components of the two points together.
	constexpr Point<T> operator+=(const Point<T>& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}

	// Subtracts the components of this point from the other.
	constexpr Point<T> operator-=(const Point<T>& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// Multiplies the components of this point by a scalar.
	constexpr Point<T> operator*=(float scale) noexcept {
		x *= scale;
		y *= scale;
		return *this;
	}

	// Multiplies the components of the two points together.
	constexpr Point<T> operator*=(const Point<T>& other) noexcept {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	// Divides the components of this point by a scalar.
	constexpr Point<T> operator/=(float scale) noexcept {
		x /= scale;
		y /= scale;
		return *this;
	}

	// Divides the components of this point by the other.
	constexpr Point<T> operator/=(const Point<T>& other) noexcept {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	// Performs a bitwise and on the two points.
	constexpr Point<T> operator&=(const Point<T>& other) noexcept {
		x &= other.x;
		y &= other.y;
		return *this;
	}

	// Performs a bitwise and on the two points.
	constexpr Point<T> operator|=(const Point<T>& other) noexcept {
		x |= other.x;
		y |= other.y;
		return *this;
	}

	// Performs a left shift on the components of this point with a scalar.
	constexpr Point<T> operator<<=(float scale) noexcept {
		x <<= scale;
		y <<= scale;
		return *this;
	}

	// Performs a left shift on the components of the two points.
	constexpr Point<T> operator<<=(const Point<T>& other) noexcept {
		x <<= other.x;
		y <<= other.y;
		return *this;
	}

	// Performs a right shift on the components of this point with a scalar.
	constexpr Point<T> operator>>=(float scale) noexcept {
		x >>= scale;
		y >>= scale;
		return *this;
	}

	// Performs a left shift on the components of the two points.
	constexpr Point<T> operator>>=(const Point<T>& other) noexcept {
		x >>= other.x;
		y >>= other.y;
		return *this;
	}

	// Compares the two points, returning true if both components are equal.
	constexpr bool operator==(const Point<T>& other) const noexcept {
		return x == other.x && y == other.y;
	}

	// Compares the two points, returning false if both components are equal.
	constexpr bool operator!=(const Point<T>& other) const noexcept {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
