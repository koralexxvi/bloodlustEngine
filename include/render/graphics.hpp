#pragma once

#include "../datatype/color.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "../util.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>

namespace Bloodlust {

SDL_GPUTextureFormat getDepthTexFormat();

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool initGraphics();

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool runRenderPass();

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool onResize(uint32_t width, uint32_t height);

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool resizeSwapchainTexture(uint32_t width, uint32_t height);

void clearAll();

void clipToRect(const SDL_Rect& rect);
void resetClip();

void drawLine(const Vector2& start, const Vector2& end, float width);

void drawRectOutline(const Vector2& pos, const Vector2& size, float width, float zIndex = 0.0f);
void drawRectOutline(const SDL_FRect& rect, float width, float zIndex = 0.0f);
void fillRect(const Vector2& pos, const Vector2& size, float zIndex = 0.0f);
void fillRect(const SDL_FRect& rect, float zIndex = 0.0f);

void drawCircleOutline(const Vector2& origin, float radius, float width);
void fillCircle(const Vector2& origin, float radius);

void drawTexture(const Texture* texture, const SDL_FRect& rect);
void drawTexture(const Texture* texture, const Vector2& pos, const Vector2& size);

void drawText(TTF_Text* textObject, const Vector2& pos, float size);

void setPixel(const Vector2& pos, float zIndex = 0.0f);

void setDrawColor(const Color& color);
void setDrawColor(uint32_t color);
void setDrawColor(float r, float g, float b, float a = 1.0f);

}  // namespace Bloodlust
