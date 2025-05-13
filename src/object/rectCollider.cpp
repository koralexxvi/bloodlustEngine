#include "../../include/object/rectCollider.hpp"

#include "../../include/datatype/vector2.hpp"
#include "../../include/object/circleCollider.hpp"

#include <cmath>

using namespace Bloodlust::Objects;
using Bloodlust::DataTypes::Vector2;

static inline constexpr bool valueInRange(float value, float min, float max) {
	return value >= min && value <= max;
}

void RectCollider::step() {}

bool RectCollider::intersects(const Vector2& other) const {
	return valueInRange(other.x, position.x - size.x, position.x + size.x)
		&& valueInRange(other.y, position.y - size.y, position.y + size.y);
}

bool RectCollider::intersects(const RectCollider& other) const {
	return (valueInRange(position.x, other.position.x, other.position.x + other.size.x)
			|| valueInRange(other.position.x, position.x, position.x + size.x))
		&& (valueInRange(position.y, other.position.y, other.position.y + other.size.y)
			|| valueInRange(other.position.y, position.y, position.y + size.y));
}

bool RectCollider::intersects(const CircleCollider& other) const {
	const float cx = std::fabsf(other.position.x - position.x - size.x);
	if (cx > size.x + other.radius) {
		return false;
	}
	const float cy = std::fabsf(other.position.y - position.y - size.y);
	if (cy > size.y + other.radius) {
		return false;
	}
	if (cx <= size.x || cy <= size.y) {
		return true;
	}
	const float x_corner_dist = cx - size.x;
	const float y_corner_dist = cy - size.y;
	const float x_corner_dist_sq = x_corner_dist * x_corner_dist;
	const float y_corner_dist_sq = y_corner_dist * y_corner_dist;
	const float max_corner_dist_sq = other.radius * other.radius;
	return x_corner_dist_sq + y_corner_dist_sq <= max_corner_dist_sq;
}
