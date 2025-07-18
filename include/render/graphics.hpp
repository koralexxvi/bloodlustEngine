#pragma once

#include "../datatype/color.hpp"
#include "../datatype/font.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"

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

void clipToRect(const SDL_FRect& rect);
void resetClip();

void drawLine(const Vector2& start, const Vector2& end);

void drawRectOutline(const Vector2& pos, const Vector2& size, float width);
void drawRectOutline(const SDL_FRect& rect, float width);
void fillRect(const Vector2& pos, const Vector2& size);
void fillRect(const SDL_FRect& rect);

void drawCircleOutline(const Vector2& origin, float radius, float width);
void fillCircle(const Vector2& origin, float radius);

void drawTexture(Texture* texture, const SDL_FRect& rect);
void drawTexture(Texture* texture, const Vector2& pos, const Vector2& size);

void drawText(const char* str, Font* font, const Vector2& pos, float size);

void setPixel(const Vector2& pos);

void setDrawColor(const Color& color);
void setDrawColor(uint32_t color);
void setDrawColor(float r, float g, float b, float a = 1.0f);

}  // namespace Bloodlust
