#include "../../include/datatype/uiPoint.hpp"

#include "../../include/math.hpp"

using Bloodlust::UIPoint, Bloodlust::Vector2;

UIPoint::UIPoint() {}

UIPoint::UIPoint(const Vector2& relativePos) : xRel(relativePos.x), yRel(relativePos.y) {}

UIPoint::UIPoint(const Vector2& relativePos, const Vector2& offsetPos)
	: xRel(relativePos.x),
	  yRel(relativePos.y),
	  xOffset(int32_t(offsetPos.x)),
	  yOffset(int32_t(offsetPos.y)) {}

UIPoint::UIPoint(float xRel, float yRel, int32_t xOffset, int32_t yOffset)
	: xRel(xRel),
	  yRel(yRel),
	  xOffset(xOffset),
	  yOffset(yOffset) {}

UIPoint::UIPoint(float xRel, float yRel) : xRel(xRel), yRel(yRel) {}

UIPoint::UIPoint(int32_t xOffset, int32_t yOffset) : xOffset(xOffset), yOffset(yOffset) {}

Vector2 UIPoint::toAbsolutePos(const Vector2& relativeTo) const {
	return {float(xOffset) + (xRel * relativeTo.x), float(yOffset) + (yRel * relativeTo.y)};
}

Vector2 UIPoint::toRelativePos(const Vector2& relativeTo) const {
	return {xRel + (float(xOffset) / relativeTo.x), yRel + (float(yOffset) / relativeTo.y)};
}

UIPoint UIPoint::lerp(const UIPoint& other, float alpha) const {
	return {
		Math::lerp(xRel, other.xRel, alpha),
		Math::lerp(yRel, other.yRel, alpha),
		int32_t(Math::lerp(float(xOffset), float(other.xOffset), alpha)),
		int32_t(Math::lerp(float(yOffset), float(other.yOffset), alpha))
	};
}

bool UIPoint::operator==(const UIPoint& other) const {
	return xRel == other.xRel && yRel == other.yRel && xOffset == other.xOffset
		&& yOffset == other.yOffset;
}

bool UIPoint::operator!=(const UIPoint& other) const {
	return !(*this == other);
}
