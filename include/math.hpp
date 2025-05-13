#ifndef BL_MATH_H
#define BL_MATH_H

#undef PI
#undef PI_F
#define PI 3.14159265358979323846
#define PI_F 3.14159265358979323846f

namespace Bloodlust::Math {

template<typename T1, typename T2> constexpr T1 lerp(T1 from, T1 to, T2 alpha) {
	return from + (to - from) * alpha;
}

template<typename A, typename I, typename O>
constexpr O map(A alpha, I inmin, I inmax, O outmin, O outmax) {
	return outmin + (alpha - inmin) * (outmax - outmin) / (inmax - inmin);
}

constexpr float toDegrees(float radians) {
	return (radians / PI_F) * 180.0f;
}

constexpr float toRadians(float degrees) {
	return (degrees / 180.0f) * PI_F;
}

constexpr double toDegrees(double radians) {
	return (radians / PI) * 180.0;
}

constexpr double toRadians(double degrees) {
	return (degrees / 180.0) * PI;
}

}  // namespace Bloodlust::Math

#endif
