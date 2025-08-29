#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "../util.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>

#define BL_CIRCLE_POINTS (32)

namespace Bloodlust {

// Returns the format of the depth texture.
SDL_GPUTextureFormat getDepthTexFormat();

// Initialises the graphics subsystem, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool initGraphics();

// Runs the render pass, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool runRenderPass();

// A callback that is fired when the window is resized, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool onResize(uint32_t width, uint32_t height);

// Resizes the swapchain texture, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool resizeSwapchainTexture(uint32_t width, uint32_t height);

// Returns the alpha between the last physics step and the next one.
double getAlphaBetweenSteps();

// Clears everything queued to be drawn.
void clearAll();

// Sets the clip rect to the provided value, not rendering anything outside that area.
void clipToRect(const SDL_Rect& rect);

// Resets the clip rect.
void resetClip();

// Draws a line between two points with the given width.
void drawLine(const Vector2& start, const Vector2& end, float width);

// Draws the outline of a rectangle with the given position,size and width.
void drawRectOutline(const Vector2& pos, const Vector2& size, float width, float zIndex = 0.0f);

// Draws the outline of a rectangle with the given rect bounds and width.
void drawRectOutline(const SDL_FRect& rect, float width, float zIndex = 0.0f);

// Fills a rectangle with the given position and size.
void fillRect(const Vector2& pos, const Vector2& size, float zIndex = 0.0f);

// Fills a rectangle with the given rect bounds.
void fillRect(const SDL_FRect& rect, float zIndex = 0.0f);

// Draws an outline of a circle with the given origin, radius and width.
void drawCircleOutline(const Vector2& origin, float radius, float width);

// Fills a circle with the given origin and radius.
void fillCircle(const Vector2& origin, float radius);

// Draws a texture to the screen.
void drawTexture(const Texture* texture, const SDL_FRect& rect);

// Draws a texture to the screen.
void drawTexture(const Texture* texture, const Vector2& pos, const Vector2& size);

// Draws a TTF_Text to the screen with the given position and font size.
void drawText(TTF_Text* textObject, const Vector2& pos, float size);

// Sets a pixel to the current draw colour.
void setPixel(const Vector2& pos, float zIndex = 0.0f);

// Sets the current draw colour.
void setDrawColor(const Colour& color);

// Sets the current draw colour.
void setDrawColor(uint32_t color);

// Sets the current draw colour.
void setDrawColor(float r, float g, float b, float a = 1.0f);

// Shuts down the graphics subsystem.
void closeGraphics();

}  // namespace Bloodlust
