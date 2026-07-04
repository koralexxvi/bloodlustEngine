#pragma once

#include "../math.hpp"

#include <cmath>
#include <format>
#include <string>

namespace Bloodlust {

// A vector type with two 32-bit float components.
struct Vector2 {
	// The X coordinate of this vector.
	float x;

	// The Y coordinate of this vector.
	float y;

	// Constructs a vector with an X and Y value of 0.0f.
	constexpr Vector2() noexcept : x(0.0f), y(0.0f) {}

	// Constructs a vector with the provided components.
	constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}

	// Constructs a unit vector from the specified angle in radians.
	static constexpr Vector2 fromAngle(float angle) noexcept {
		return {std::sin(angle), std::cos(angle)};
	}

	// Returns the angle that this vector points towards in radians.
	constexpr float toAngle() const noexcept {
		return std::atan2(y, x) + BL_PI_F;
	}

	// Converts this vector to a string.
	inline std::string toString() const noexcept {
		return std::format("<{}, {}>", x, y);
	}

	// Returns the dot product of this vector.
	constexpr float dot(const Vector2& other) const noexcept {
		return x * other.x + y * other.y;
	}

	// Returns the squared magnitude of this vector.
	constexpr float magnitudeSquared() const noexcept {
		return x * x + y * y;
	}

	// Returns the magnitude of this vector.
	constexpr float magnitude() const noexcept {
		return std::sqrt(magnitudeSquared());
	}

	// Returns the squared distance between this vector and another.
	constexpr float distanceSquared(const Vector2& other) const noexcept {
		return (*this - other).magnitudeSquared();
	}

	// Returns the distance between this vector and another.
	constexpr float distance(const Vector2& other) const noexcept {
		return std::sqrt(distanceSquared(other));
	}

	// Returns this vector with both components floored.
	constexpr Vector2 floor() const noexcept {
		return {std::floor(x), std::floor(y)};
	}

	// Returns this vector with a magnitude of 1.0f. If both components are zero, this will result
	// in a vector of <NaN, NaN>.
	constexpr Vector2 normalize() const noexcept {
		return *this / magnitude();
	}

	// Returns this vector linearly interpolated towards another.
	constexpr Vector2 lerp(const Vector2& other, float alpha) const noexcept {
		return {std::lerp(this->x, other.x, alpha), std::lerp(this->y, other.y, alpha)};
	}

	// Returns true if this vector is within epsilon units of the other vector provided.
	constexpr bool fuzzyEq(const Vector2& other, float epsilon) const noexcept {
		return Bloodlust::fuzzyEq(x, other.x, epsilon) && Bloodlust::fuzzyEq(y, other.y, epsilon);
	}

	// Returns true if either of the components are NaN.
	constexpr bool isNaN() const noexcept {
		return x != x || y != y;
	}

	// Returns this vector.
	constexpr Vector2 operator+() const noexcept {
		return *this;
	}

	// Returns the unary negation of this vector.
	constexpr Vector2 operator-() const noexcept {
		return {-x, -y};
	}

	// Adds the components of the two vectors together.
	constexpr Vector2 operator+(const Vector2& other) const noexcept {
		return {x + other.x, y + other.y};
	}

	// Subtracts the components of this vector from the other.
	constexpr Vector2 operator-(const Vector2& other) const noexcept {
		return {x - other.x, y - other.y};
	}

	// Multiplies the components of this vector by a scalar.
	constexpr Vector2 operator*(float scale) const noexcept {
		return {x * scale, y * scale};
	}

	// Multiplies the components of the two vectors together.
	constexpr Vector2 operator*(const Vector2& other) const noexcept {
		return {x * other.x, y * other.y};
	}

	// Divides the components of this vector by a scalar.
	constexpr Vector2 operator/(float scale) const noexcept {
		return {x / scale, y / scale};
	}

	// Divides the components of this vector by the other.
	constexpr Vector2 operator/(const Vector2& other) const noexcept {
		return {x / other.x, y / other.y};
	}

	// Adds the components of the two vectors together.
	constexpr Vector2 operator+=(const Vector2& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}

	// Subtracts the components of this vector from the other.
	constexpr Vector2 operator-=(const Vector2& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// Multiplies the components of this vector by a scalar.
	constexpr Vector2 operator*=(float scale) noexcept {
		x *= scale;
		y *= scale;
		return *this;
	}

	// Multiplies the components of the two vectors together.
	constexpr Vector2 operator*=(const Vector2& other) noexcept {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	// Divides the components of this vector by a scalar.
	constexpr Vector2 operator/=(float scale) noexcept {
		x /= scale;
		y /= scale;
		return *this;
	}

	// Divides the components of this vector by the other.
	constexpr Vector2 operator/=(const Vector2& other) noexcept {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	// Compares the two vectors, returning true if both components are equal.
	constexpr bool operator==(const Vector2& other) const noexcept {
		return x == other.x && y == other.y;
	}

	// Compares the two vectors, returning false if both components are equal.
	constexpr bool operator!=(const Vector2& other) const noexcept {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
