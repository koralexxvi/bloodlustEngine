#pragma once

#include <cmath>

#define BL_PI (3.14159265358979323846)
#define BL_PI_F (3.14159265358979323846f)

namespace Bloodlust {

// Returns the alpha value mapped from inmin and inmax to outmin and outmax.
constexpr float map(float alpha, float inmin, float inmax, float outmin, float outmax) {
	return outmin + (alpha - inmin) * (outmax - outmin) / (inmax - inmin);
}

// Returns the alpha value mapped from inmin and inmax to outmin and outmax.
constexpr double map(double alpha, double inmin, double inmax, double outmin, double outmax) {
	return outmin + (alpha - inmin) * (outmax - outmin) / (inmax - inmin);
}

// Converts the given angle from radians to degrees.
constexpr float toDegrees(float radians) {
	return (radians / BL_PI_F) * 180.0f;
}

// Converts the given angle from radians to degrees.
constexpr double toDegrees(double radians) {
	return (radians / BL_PI) * 180.0;
}

// Converts the given angle from degrees to radians.
constexpr float toRadians(float degrees) {
	return (degrees / 180.0f) * BL_PI_F;
}

// Converts the given angle from degrees to radians.
constexpr double toRadians(double degrees) {
	return (degrees / 180.0) * BL_PI;
}

// Returns true if a and b are within epsilon units to eachother.
constexpr bool fuzzyEq(float a, float b, float epsilon) {
	return std::abs(a - b) < epsilon;
}

// Returns true if a and b are within epsilon units to eachother.
constexpr bool fuzzyEq(double a, double b, double epsilon) {
	return std::abs(a - b) < epsilon;
}

// Returns true if value is both greater or equal to min and less than or equal to max.
constexpr bool valueInRange(float value, float min, float max) {
	return value >= min && value <= max;
}

}  // namespace Bloodlust
