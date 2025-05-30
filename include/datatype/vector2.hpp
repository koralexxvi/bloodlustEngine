#ifndef BL_DATA_VECTOR2_H
#define BL_DATA_VECTOR2_H

#include <string>

namespace Bloodlust {

struct Vector2 {
	float x, y;
	Vector2();
	Vector2(float x, float y);

	static Vector2 fromAngle(float angle);
	float toAngle() const;
	std::string toString() const;

	float dot(const Vector2& other) const;
	float magnitudeSquared() const;
	float magnitude() const;
	float distanceSquared(const Vector2& other) const;
	float distance(const Vector2& other) const;
	Vector2 floor() const;
	Vector2 normalize() const;
	Vector2 lerp(const Vector2& other, float alpha) const;

	bool fuzzyEq(const Vector2& other, float epsilon) const;
	bool isNaN() const;

	Vector2 operator+() const;
	Vector2 operator-() const;

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const Vector2& other) const;
	Vector2 operator/(const Vector2& other) const;
	template<typename T> Vector2 operator*(const T scale) const {
		return {x * float(scale), y * float(scale)};
	}
	template<typename T> Vector2 operator/(const T scale) const {
		return {x / float(scale), y / float(scale)};
	}

	Vector2 operator+=(const Vector2& other);
	Vector2 operator-=(const Vector2& other);
	Vector2 operator*=(const Vector2& other);
	Vector2 operator/=(const Vector2& other);
	template<typename T> Vector2 operator*=(const T scale) const {
		x *= scale;
		y *= scale;
		return *this;
	}
	template<typename T> Vector2 operator/=(const T scale) const {
		x /= scale;
		y /= scale;
		return *this;
	}

	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;
};

}  // namespace Bloodlust

#endif
