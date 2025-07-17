#pragma once

#include "../render/video.hpp"
#include "vector2.hpp"

#include <cstdint>
#include <string>

namespace Bloodlust {

struct UIPoint {
	float xRel = 0.0f;
	float yRel = 0.0f;
	int32_t xOffset = 0;
	int32_t yOffset = 0;

	constexpr UIPoint() {}

	constexpr UIPoint(const Vector2& relativePos) : xRel(relativePos.x), yRel(relativePos.y) {}
	constexpr UIPoint(const Vector2& relativePos, const Vector2& offsetPos)
		: xRel(relativePos.x),
		  yRel(relativePos.y),
		  xOffset(int32_t(offsetPos.x)),
		  yOffset(int32_t(offsetPos.y)) {}

	constexpr UIPoint(float xRel, float yRel, int32_t xOffset, int32_t yOffset)
		: xRel(xRel),
		  yRel(yRel),
		  xOffset(xOffset),
		  yOffset(yOffset) {}

	constexpr UIPoint(float xRel, float yRel) : xRel(xRel), yRel(yRel) {}
	constexpr UIPoint(int32_t xOffset, int32_t yOffset) : xOffset(xOffset), yOffset(yOffset) {}

	constexpr Vector2 toAbsolutePos(const Vector2& relativeTo = getResolution()) const {
		return {float(xOffset) + (xRel * relativeTo.x), float(yOffset) + (yRel * relativeTo.y)};
	}

	constexpr Vector2 toRelativePos(const Vector2& relativeTo = getResolution()) const {
		return {xRel + (float(xOffset) / relativeTo.x), yRel + (float(yOffset) / relativeTo.y)};
	}

	constexpr std::string toString() const {
		return std::format("UIPoint(<{}, {}>, <{}, {}>)", xRel, yRel, xOffset, yOffset);
	}

	constexpr UIPoint lerp(const UIPoint& other, float alpha) const {
		return {
			std::lerp(xRel, other.xRel, alpha),
			std::lerp(yRel, other.yRel, alpha),
			int32_t(std::lerp(float(xOffset), float(other.xOffset), alpha)),
			int32_t(std::lerp(float(yOffset), float(other.yOffset), alpha))
		};
	}

	constexpr bool fuzzyEq(const UIPoint& other, float epsilon) const {
		return xOffset == other.xOffset && yOffset == other.yOffset
			&& Bloodlust::fuzzyEq(xRel, other.xRel, epsilon)
			&& Bloodlust::fuzzyEq(yRel, other.yRel, epsilon);
	}

	constexpr bool operator==(const UIPoint& other) const {
		return xRel == other.xRel && yRel == other.yRel && xOffset == other.xOffset
			&& yOffset == other.yOffset;
	}

	constexpr bool operator!=(const UIPoint& other) const {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
