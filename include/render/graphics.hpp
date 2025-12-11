#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/point.hpp"
#include "../datatype/texture.hpp"
#include "../datatype/vector2.hpp"
#include "../util.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>

#define BL_CIRCLE_POINTS (32)

namespace Bloodlust {

// Initialises the graphics subsystem, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool initGraphics();

// Runs the render pass, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool runRenderPass();

// A callback that is fired when the window is resized, returning true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool onResize(std::uint32_t width, std::uint32_t height);

// Returns the alpha between the last physics step and the next one.
double getAlphaBetweenSteps();

// Clears everything queued to be drawn.
void clearAll();

// Returns a reference to the current drawing resolution.
const Point<std::uint32_t>& getDrawResolution();

// Sets the current drawing resolution.
void setDrawResolution(const Point<std::uint32_t>& newRes);

// Sets the clip rect to the provided value, not rendering anything outside that area.
void clipToRect(const SDL_Rect& rect);

// Resets the clip rect.
void resetClip();

// Draws a line to a layer between two points with the given width.
void drawLine(const Vector2& start, const Vector2& end, float width);

// Draws the outline of a rectangle to a layer with the given position, size and width.
void drawRectOutline(const Vector2& pos, const Vector2& size, float width);

// Draws the outline of a rectangle to a layer with the given rect bounds and width.
void drawRectOutline(const SDL_FRect& rect, float width);

// Fills a rectangle to a layer with the given position and size.
void fillRect(const Vector2& pos, const Vector2& size);

// Fills a rectangle to a layer with the given rect bounds.
void fillRect(const SDL_FRect& rect);

// Draws an outline of a circle to a layer with the given origin, radius and width.
void drawCircleOutline(const Vector2& origin, float radius, float width);

// Fills a circle to a layer with the given origin and radius.
void fillCircle(const Vector2& origin, float radius);

// Draws a texture to a layer.
void drawTexture(const Texture* texture, const SDL_FRect& rect);

// Draws a texture to a layer.
void drawTexture(const Texture* texture, const SDL_FRect& rect, const SDL_FRect& texClip);

// Draws a texture to a layer.
void drawTexture(const Texture* texture, const Vector2& pos, const Vector2& size);

// Draws a texture to a layer.
void drawTexture(
	const Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const SDL_FRect& texClip
);

// Draws a TTF_Text to a layer with the given position and font size.
void drawText(TTF_Text* textObject, const Vector2& pos, float size);

// Sets a pixel of a layer to the current draw colour.
void setPixel(const Vector2& pos);

// Returns the current draw colour.
Colour getDrawColor();

// Sets the current draw colour.
void setDrawColour(const Colour& colour);

// Sets the current draw colour.
void setDrawColour(std::uint32_t colour);

// Sets the current draw colour.
void setDrawColour(float r, float g, float b, float a = 1.0f);

// Returns the current draw layer.
std::size_t getDrawLayer();

// Sets the current draw layer. If this is greater than or equal to BL_LAYER_COUNT, the behaviour is
// undefined.
void setDrawLayer(std::size_t newLayer);

// Shuts down the graphics subsystem.
void closeGraphics();

}  // namespace Bloodlust
