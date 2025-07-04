#include "../../include/object/circleCollider.hpp"

#include "../../include/datatype/vector2.hpp"
#include "../../include/object/rectCollider.hpp"

using Bloodlust::CircleCollider;

void CircleCollider::step() {}

bool CircleCollider::intersects(const Vector2& other) const {
	return position.distanceSquared(other) <= (radius * radius);
}

bool CircleCollider::intersects(const CircleCollider& other) const {
	const float radius_sum = radius + other.radius;
	return position.distanceSquared(other.position) <= radius_sum * radius_sum;
}

bool CircleCollider::intersects(const RectCollider& other) const {
	return other.intersects(*this);
}
