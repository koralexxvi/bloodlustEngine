#pragma once

#include <cmath>
#define BL_PI (3.14159265358979323846)
#define BL_PI_F (3.14159265358979323846f)

namespace Bloodlust {

template<typename T> constexpr T map(T alpha, T inmin, T inmax, T outmin, T outmax) {
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

}  // namespace Bloodlust
