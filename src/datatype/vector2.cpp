#include "../../include/datatype/vector2.hpp"

#include "../../include/math.hpp"

#include <cmath>

using Bloodlust::DataTypes::Vector2;

Vector2::Vector2() : x(0.0f), y(0.0f) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::fromAngle(float angle) {
	return {std::sinf(angle), std::cosf(angle)};
}

float Vector2::toAngle() const {
	return std::atan2f(y, x) + PI_F;
}

float Vector2::dot(const Vector2& other) const {
	return x * other.x + y * other.y;
}

float Vector2::magnitudeSquared() const {
	return x * x + y * y;
}

float Vector2::magnitude() const {
	return std::sqrtf(magnitudeSquared());
}

float Vector2::distanceSquared(const Vector2& other) const {
	return (*this - other).magnitudeSquared();
}

float Vector2::distance(const Vector2& other) const {
	return std::sqrtf(distanceSquared(other));
}

Vector2 Vector2::normalize() const {
	return *this / magnitude();
}

Vector2 Vector2::floor() const {
	return {std::floorf(x), std::floorf(y)};
}

Vector2 Vector2::lerp(const Vector2& other, float alpha) const {
	return {this->x + (other.x - this->x) * alpha, this->y + (other.y - this->y) * alpha};
}

bool Vector2::isNaN() const {
	return x != x || y != y;
}

Vector2 Vector2::operator+() const {
	return *this;
}

Vector2 Vector2::operator-() const {
	return {-x, -y};
}

Vector2 Vector2::operator+(const Vector2& other) const {
	return {x + other.x, y + other.y};
}

Vector2 Vector2::operator-(const Vector2& other) const {
	return {x - other.x, y - other.y};
}

Vector2 Vector2::operator*(const Vector2& other) const {
	return {x * other.x, y * other.y};
}

Vector2 Vector2::operator/(const Vector2& other) const {
	return {x / other.x, y / other.y};
}

Vector2 Vector2::operator+=(const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2 Vector2::operator*=(const Vector2& other) {
	x *= other.x;
	y *= other.y;
	return *this;
}

Vector2 Vector2::operator/=(const Vector2& other) {
	x /= other.x;
	y /= other.y;
	return *this;
}

bool Vector2::operator==(const Vector2& other) const {
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const {
	return !(*this == other);
}
