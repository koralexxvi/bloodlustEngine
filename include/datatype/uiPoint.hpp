#pragma once

#include "../render/graphics.hpp"
#include "vector2.hpp"

#include <cstdint>
#include <string>

namespace Bloodlust {

// A structure consisting of two 32-bit floats representing a relative offset, and two signed 32-bit
// integers representing an absolute offset.
struct UIPoint {
	// The relative X offset.
	float xRel = 0.0f;

	// The relative Y offset.
	float yRel = 0.0f;

	// The absolute X offset.
	std::int32_t xOffset = 0;

	// The absolute Y offset.
	std::int32_t yOffset = 0;

	// Constructs a UIPoint with all components set to zero.
	constexpr UIPoint() {}

	// Constructs a UIPoint with the relative components set to the provided vector.
	constexpr UIPoint(const Vector2& relativePos) : xRel(relativePos.x), yRel(relativePos.y) {}

	// Constructs a UIPoint with the relative components set to the first vector, and the absolute
	// components set to the components of the second vector converted to integers.
	constexpr UIPoint(const Vector2& relativePos, const Vector2& absolutePos)
		: xRel(relativePos.x),
		  yRel(relativePos.y),
		  xOffset(static_cast<std::int32_t>(absolutePos.x)),
		  yOffset(static_cast<std::int32_t>(absolutePos.y)) {}

	// Constructs a UIPoint with the relative components set to the two floats, and the
	// absolute components set to the two integers.
	constexpr UIPoint(float xRel, float yRel, std::int32_t xAbs, std::int32_t yAbs)
		: xRel(xRel),
		  yRel(yRel),
		  xOffset(xAbs),
		  yOffset(yAbs) {}

	// Constructs a UIPoint with the relative components set to the two floats.
	constexpr UIPoint(float xRel, float yRel) : xRel(xRel), yRel(yRel) {}

	// Constructs a UIPoint with the absolute components set to the two integers.
	constexpr UIPoint(std::int32_t xAbs, std::int32_t yAbs) : xOffset(xAbs), yOffset(yAbs) {}

	// Returns a Vector2 representing this UIPoint converted to a relative position. If a relative
	// size is not specified, the window resolution is used.
	constexpr Vector2 toRelativePos(
		const Vector2& relativeTo = getDrawResolution().toVector()
	) const {
		return {
			xRel + (static_cast<float>(xOffset) / relativeTo.x),
			yRel + (static_cast<float>(yOffset) / relativeTo.y)
		};
	}

	// Returns a Vector2 representing this UIPoint converted to an absolute position. If a relative
	// size is not specified, the window resolution is used.
	constexpr Vector2 toAbsolutePos(
		const Vector2& relativeTo = getDrawResolution().toVector()
	) const {
		return {
			static_cast<float>(xOffset) + (xRel * relativeTo.x),
			static_cast<float>(yOffset) + (yRel * relativeTo.y)
		};
	}

	// Converts this UIPoint to a string.
	constexpr std::string toString() const {
		return std::format("UIPoint(<{}, {}>, <{}, {}>)", xRel, yRel, xOffset, yOffset);
	}

	// Returns this UIPoint linearly interpolated towards another.
	constexpr UIPoint lerp(const UIPoint& other, float alpha) const {
		return {
			std::lerp(xRel, other.xRel, alpha),
			std::lerp(yRel, other.yRel, alpha),
			static_cast<std::int32_t>(
				std::lerp(static_cast<float>(xOffset), static_cast<float>(other.xOffset), alpha)
			),
			static_cast<std::int32_t>(
				std::lerp(static_cast<float>(yOffset), static_cast<float>(other.yOffset), alpha)
			)
		};
	}

	// Returns true if both points' absolute offsets are equal, and the relative components are
	// within epsilon units to eachother.
	constexpr bool fuzzyEq(const UIPoint& other, float epsilon) const {
		return xOffset == other.xOffset && yOffset == other.yOffset
			&& Bloodlust::fuzzyEq(xRel, other.xRel, epsilon)
			&& Bloodlust::fuzzyEq(yRel, other.yRel, epsilon);
	}

	// Compares both points and returns true if they are all equal.
	constexpr bool operator==(const UIPoint& other) const {
		return xRel == other.xRel && yRel == other.yRel && xOffset == other.xOffset
			&& yOffset == other.yOffset;
	}

	// Compares both points and returns false if they are all equal.
	constexpr bool operator!=(const UIPoint& other) const {
		return !(*this == other);
	}
};

}  // namespace Bloodlust
