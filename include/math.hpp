#pragma once

#include <cmath>
#define BL_PI (3.14159265358979323846)
#define BL_PI_F (3.14159265358979323846f)

namespace Bloodlust {

constexpr float map(float alpha, float inmin, float inmax, float outmin, float outmax) {
	return outmin + (alpha - inmin) * (outmax - outmin) / (inmax - inmin);
}
constexpr double map(double alpha, double inmin, double inmax, double outmin, double outmax) {
	return outmin + (alpha - inmin) * (outmax - outmin) / (inmax - inmin);
}

constexpr float toDegrees(float radians) {
	return (radians / BL_PI_F) * 180.0f;
}
constexpr double toDegrees(double radians) {
	return (radians / BL_PI) * 180.0;
}

constexpr float toRadians(float degrees) {
	return (degrees / 180.0f) * BL_PI_F;
}
constexpr double toRadians(double degrees) {
	return (degrees / 180.0) * BL_PI;
}

constexpr bool fuzzyEq(float a, float b, float epsilon) {
	return std::fabsf(a - b) < epsilon;
}
constexpr bool fuzzyEq(double a, double b, double epsilon) {
	return std::fabs(a - b) < epsilon;
}

constexpr bool valueInRange(float value, float min, float max) {
	return value >= min && value <= max;
}

}  // namespace Bloodlust
