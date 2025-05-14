#include "../include/graphics.hpp"

#include "../include/app.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace Bloodlust;

bool Graphics::drawRect(const Vector2& pos, const Vector2& size) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	return SDL_RenderRect(app->renderer, &drawRect);
}

bool Graphics::fillRect(const Vector2& pos, const Vector2& size) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	return SDL_RenderFillRect(app->renderer, &drawRect);
}

bool Graphics::fillCircle(const Vector2& origin, float radius) {
	int min_x = int(origin.x - radius), max_x = int(origin.x + radius);
	int min_y = int(origin.y - radius), max_y = int(origin.y + radius);
	float radius_squared = radius * radius;
	bool success = true;
	for (int x = min_x; x <= max_x; x++) {
		float fx = float(x);
		for (int y = min_y; y <= max_y; y++) {
			float fy = float(y);
			if (origin.distanceSquared({fx, fy}) < radius_squared) {
				success &= SDL_RenderPoint(app->renderer, fx, fy);
			}
		}
	}
	return success;
}

bool Graphics::drawTexture(Texture* texture, const Vector2& pos, const Vector2& size) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	return SDL_RenderTexture(app->renderer, texture->getData(), nullptr, &drawRect);
}

bool Graphics::drawTexture(
	Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const Vector2& clipPos,
	const Vector2& clipSize
) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	SDL_FRect clipRect = {clipPos.x, clipPos.y, clipSize.x, clipSize.y};
	return SDL_RenderTexture(app->renderer, texture->getData(), &clipRect, &drawRect);
}

void Graphics::setDrawColor(uint32_t color) {
	SDL_SetRenderDrawColor(
		app->renderer,
		uint8_t(color & 0xFF),
		uint8_t(color & 0xFF00),
		uint8_t(color & 0xFF0000),
		uint8_t(color & 0xFF00000)
	);
}

void Graphics::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	SDL_SetRenderDrawColor(app->renderer, r, g, b, a);
}

void Graphics::setDrawColor(float r, float g, float b, float a) {
	SDL_SetRenderDrawColorFloat(app->renderer, r, g, b, a);
}
