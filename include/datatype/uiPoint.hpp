#ifndef BL_DATA_UIPOINT_H
#define BL_DATA_UIPOINT_H

#include "../video.hpp"
#include "vector2.hpp"

#include <cstdint>

namespace Bloodlust {

struct UIPoint {
	float xRel = 0.0f;
	float yRel = 0.0f;
	int32_t xOffset = 0;
	int32_t yOffset = 0;

	UIPoint();

	UIPoint(const Vector2& relativePos);
	UIPoint(const Vector2& relativePos, const Vector2& offsetPos);

	UIPoint(float xRel, float yRel, int32_t xOffset, int32_t yOffset);
	UIPoint(float xRel, float yRel);
	UIPoint(int32_t xOffset, int32_t yOffset);

	Vector2 toAbsolutePos(const Vector2& relativeTo = Video::getResolution()) const;
	Vector2 toRelativePos(const Vector2& relativeTo = Video::getResolution()) const;

	UIPoint lerp(const UIPoint& other, float alpha) const;
};

}  // namespace Bloodlust

#endif
