#ifndef BL_GRAPHICS_H
#define BL_GRAPHICS_H

#include "datatype/color.hpp"
#include "datatype/texture.hpp"
#include "datatype/vector2.hpp"

#include <cstdint>

namespace Bloodlust::Graphics {
using namespace Bloodlust::DataTypes;

enum class BlendMode {
	NONE,
	BLEND,
	BLEND_PREMUL,
	ADD,
	ADD_PREMUL,
	MODULATE,
	MULTIPLY
};

bool clearAll();

bool clipToRect(const Vector2& pos, const Vector2& size);
bool resetClip();

bool drawLine(const Vector2& start, const Vector2& end);

bool drawRect(const Vector2& pos, const Vector2& size);
bool fillRect(const Vector2& pos, const Vector2& size);
// TODO: drawCircle
bool fillCircle(const Vector2& origin, float radius);

bool drawTexture(Texture* texture, const Vector2& pos, const Vector2& size);
bool drawTexture(
	Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const Vector2& clipPos,
	const Vector2& clipSize
);

bool setPixel(const Vector2& pos);

bool setDrawBlendMode(BlendMode mode);

void setDrawColor(uint32_t color);
void setDrawColor(const DataTypes::Color& color);
void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
void setDrawColor(float r, float g, float b, float a = 1.0f);

}  // namespace Bloodlust::Graphics

#endif
