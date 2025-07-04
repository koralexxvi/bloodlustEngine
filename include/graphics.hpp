#ifndef BL_GRAPHICS_H
#define BL_GRAPHICS_H

#include "datatype/color.hpp"
#include "datatype/texture.hpp"
#include "datatype/vector2.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <cstdint>

namespace Bloodlust {

enum class BlendMode {
	NONE,
	BLEND,
	BLEND_PREMUL,
	ADD,
	ADD_PREMUL,
	MODULATE,
	MULTIPLY
};

SDL_GPUTextureFormat getDepthTexFormat();

bool initGraphics();

bool runRenderPass();

bool onResize(uint32_t width, uint32_t height);
bool resizeSwapchainTexture(uint32_t width, uint32_t height);

void clearAll();

void clipToRect(const Vector2& pos, const Vector2& size);
void resetClip();

void drawLine(const Vector2& start, const Vector2& end);

void drawRect(const Vector2& pos, const Vector2& size);
void drawRect(const SDL_FRect& rect);
void fillRect(const Vector2& pos, const Vector2& size);
void fillRect(const SDL_FRect& rect);

void drawCircle(const Vector2& origin, float radius);
void fillCircle(const Vector2& origin, float radius);

void drawTexture(Texture* texture, const SDL_FRect& rect);
void drawTexture(Texture* texture, const Vector2& pos, const Vector2& size);
void drawTexture(Texture* texture, const SDL_FRect& rect, const SDL_FRect& clipRect);
void drawTexture(
	Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const Vector2& clipPos,
	const Vector2& clipSize
);

void setPixel(const Vector2& pos);

void setDrawColor(const Color& color);
void setDrawColor(uint32_t color);
void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
void setDrawColor(float r, float g, float b, float a = 1.0f);

}  // namespace Bloodlust

#endif
