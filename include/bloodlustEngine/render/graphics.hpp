#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>

#define BL_CIRCLE_POINTS (32)

namespace Bloodlust {

struct Colour;
struct Vector2;
class Texture;

// Initialises the graphics subsystem. Will throw InternalError on failure.
void initGraphics();

// Runs the render pass. Will throw InternalError on failure.
void runRenderPass();

// A callback that is fired when the window is resized.
void onResize(std::uint32_t width, std::uint32_t height) noexcept;

// Returns the alpha between the last physics step and the next one.
double getAlphaBetweenSteps() noexcept;

// Clears everything queued to be drawn.
void clearAll() noexcept;

// Sets the clip rect to the provided value, not rendering anything outside that area.
void clipToRect(const SDL_Rect& rect) noexcept;

// Resets the clip rect.
void resetClip() noexcept;

// Draws a line to a layer between two points with the given width.
void drawLine(const Vector2& start, const Vector2& end, float width) noexcept;

// Draws the outline of a rectangle to a layer with the given position, size and width.
void drawRectOutline(const Vector2& pos, const Vector2& size, float width) noexcept;

// Draws the outline of a rectangle to a layer with the given rect bounds and width.
void drawRectOutline(const SDL_FRect& rect, float width) noexcept;

// Fills a rectangle to a layer with the given position and size.
void fillRect(const Vector2& pos, const Vector2& size) noexcept;

// Fills a rectangle to a layer with the given rect bounds.
void fillRect(const SDL_FRect& rect) noexcept;

// Draws an outline of a circle to a layer with the given origin, radius and width.
void drawCircleOutline(const Vector2& origin, float radius, float width) noexcept;

// Fills a circle to a layer with the given origin and radius.
void fillCircle(const Vector2& origin, float radius) noexcept;

// Draws a texture to a layer.
void drawTexture(const Texture* texture, const SDL_FRect& rect) noexcept;

// Draws a texture to a layer.
void drawTexture(const Texture* texture, const SDL_FRect& rect, const SDL_FRect& texClip) noexcept;

// Draws a texture to a layer.
void drawTexture(const Texture* texture, const Vector2& pos, const Vector2& size) noexcept;

// Draws a texture to a layer.
void drawTexture(
	const Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const SDL_FRect& texClip
) noexcept;

// Draws a TTF_Text to a layer with the given position and font size.
void drawText(TTF_Text* textObject, const Vector2& pos, float size) noexcept;

// Sets a pixel of a layer to the current draw colour.
void setPixel(const Vector2& pos) noexcept;

// Returns the current draw colour.
Colour getDrawColor() noexcept;

// Sets the current draw colour.
void setDrawColour(const Colour& colour) noexcept;

// Sets the current draw colour.
void setDrawColour(std::uint32_t colour) noexcept;

// Sets the current draw colour.
void setDrawColour(float r, float g, float b, float a = 1.0f) noexcept;

// Returns the current draw layer.
std::size_t getDrawLayer() noexcept;

// Sets the current draw layer. If this is greater than or equal to BL_LAYER_COUNT, the behaviour is
// undefined.
void setDrawLayer(std::size_t newLayer) noexcept;

// Shuts down the graphics subsystem.
void closeGraphics() noexcept;

}  // namespace Bloodlust
