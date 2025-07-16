#pragma once

#include "../math.hpp"

#include <cmath>
#include <format>
#include <string>

namespace Bloodlust {

struct Vector2 {
	float x, y;
	constexpr Vector2() : x(0.0f), y(0.0f) {}
	constexpr Vector2(float x, float y) : x(x), y(y) {}

	static constexpr Vector2 fromAngle(float angle) {
		return {std::sinf(angle), std::cosf(angle)};
	}

	constexpr float toAngle() const {
		return std::atan2f(y, x) + BL_PI_F;
	}

	constexpr std::string toString() const {
		return std::format("<{}, {}>", x, y);
	}

	constexpr float dot(const Vector2& other) const {
		return x * other.x + y * other.y;
	}

	constexpr float magnitudeSquared() const {
		return x * x + y * y;
	}

	constexpr float magnitude() const {
		return std::sqrtf(magnitudeSquared());
	}

	constexpr float distanceSquared(const Vector2& other) const {
		return (*this - other).magnitudeSquared();
	}

	constexpr float distance(const Vector2& other) const {
		return std::sqrtf(distanceSquared(other));
	}

	constexpr Vector2 floor() const {
		return {std::floorf(x), std::floorf(y)};
	}

	constexpr Vector2 normalize() const {
		return *this / magnitude();
	}

	constexpr Vector2 lerp(const Vector2& other, float alpha) const {
		return {std::lerp(this->x, other.x, alpha), std::lerp(this->y, other.y, alpha)};
	}

	constexpr bool fuzzyEq(const Vector2& other, float epsilon) const {
		return Bloodlust::fuzzyEq(x, other.x, epsilon) && Bloodlust::fuzzyEq(y, other.y, epsilon);
	}

	constexpr bool isNaN() const {
		return x != x || y != y;
	}

	constexpr Vector2 operator+() const {
		return *this;
	}

	constexpr Vector2 operator-() const {
		return {-x, -y};
	}

	constexpr Vector2 operator+(const Vector2& other) const {
		return {x + other.x, y + other.y};
	}

	constexpr Vector2 operator-(const Vector2& other) const {
		return {x - other.x, y - other.y};
	}

	constexpr Vector2 operator*(float scale) const {
		return {x * scale, y * scale};
	}

	constexpr Vector2 operator*(const Vector2& other) const {
		return {x * other.x, y * other.y};
	}

	constexpr Vector2 operator/(float scale) const {
		return {x / scale, y / scale};
	}

	constexpr Vector2 operator/(const Vector2& other) const {
		return {x / other.x, y / other.y};
	}

	constexpr Vector2 operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	constexpr Vector2 operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	constexpr Vector2 operator*=(float scale) {
		x *= scale;
		y *= scale;
		return *this;
	}

	constexpr Vector2 operator*=(const Vector2& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	constexpr Vector2 operator/=(float scale) {
		x /= scale;
		y /= scale;
		return *this;
	}

	constexpr Vector2 operator/=(const Vector2& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	constexpr bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	constexpr bool operator!=(const Vector2& other) const {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
