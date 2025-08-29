#pragma once

#include "datatype/vector2.hpp"

namespace Bloodlust {

// Returns true if the given rect and point intersect.
constexpr bool rectIntersectsPoint(
	const Vector2& rectPos,
	const Vector2& rectSize,
	const Vector2& point
) {
	return valueInRange(point.x, rectPos.x - rectSize.x, rectPos.x + rectSize.x)
		&& valueInRange(point.y, rectPos.y - rectSize.y, rectPos.y + rectSize.y);
}

// Returns true if the given circle intersects a point.
constexpr bool circleIntersectsPoint(const Vector2& circlePos, float radius, const Vector2& point) {
	return circlePos.distanceSquared(point) <= (radius * radius);
}

// Returns true if the two rects intersect.
constexpr bool rectIntersectsRect(
	const Vector2& posA,
	const Vector2& sizeA,
	const Vector2& posB,
	const Vector2& sizeB
) {
	return (valueInRange(posA.x, posB.x, posB.x + sizeB.x)
			|| valueInRange(posB.x, posA.x, posA.x + sizeA.x))
		&& (valueInRange(posA.y, posB.y, posB.y + sizeB.y)
			|| valueInRange(posB.y, posA.y, posA.y + sizeA.y));
}

// Returns true if the rect and circle intersect with eachother.
constexpr bool rectIntersectsCircle(
	const Vector2& rectPos,
	const Vector2& rectSize,
	const Vector2& circlePos,
	float radius
) {
	const float cx = std::fabsf(circlePos.x - rectPos.x - rectSize.x);
	if (cx > rectSize.x + radius) {
		return false;
	}
	const float cy = std::fabsf(circlePos.y - rectPos.y - rectSize.y);
	if (cy > rectSize.y + radius) {
		return false;
	}
	if (cx <= rectSize.x || cy <= rectSize.y) {
		return true;
	}
	float xDist = cx - rectSize.x;
	float yDist = cy - rectSize.y;
	float xDistSq = xDist * xDist;
	float yDistSq = yDist * yDist;
	float maxDistSq = radius * radius;
	return xDistSq + yDistSq <= maxDistSq;
}

// Returns true if the two circles intersect.
constexpr bool circleIntersectsCircle(
	const Vector2& posA,
	float radiusA,
	const Vector2& posB,
	float radiusB
) {
	float radSum = radiusA + radiusB;
	return posA.distanceSquared(posB) <= radSum * radSum;
}

}  // namespace Bloodlust
