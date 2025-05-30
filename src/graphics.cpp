#include "../include/graphics.hpp"

#include "../include/app.hpp"

#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

bool Bloodlust::Graphics::clearAll() {
	return SDL_RenderClear(app->renderer);
}

bool Bloodlust::Graphics::clipToRect(const Vector2& pos, const Vector2& size) {
	SDL_Rect r = {int(pos.x), int(pos.y), int(size.x), int(size.y)};
	return SDL_SetRenderClipRect(app->renderer, &r);
}

bool Bloodlust::Graphics::resetClip() {
	return SDL_SetRenderClipRect(app->renderer, nullptr);
}

bool Bloodlust::Graphics::drawLine(const Vector2& start, const Vector2& end) {
	return SDL_RenderLine(app->renderer, start.x, start.y, end.x, end.y);
}

bool Bloodlust::Graphics::drawRect(const Vector2& pos, const Vector2& size) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	return SDL_RenderRect(app->renderer, &drawRect);
}

bool Bloodlust::Graphics::drawRect(const SDL_FRect* rect) {
	return SDL_RenderRect(app->renderer, rect);
}

bool Bloodlust::Graphics::fillRect(const Vector2& pos, const Vector2& size) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	return SDL_RenderFillRect(app->renderer, &drawRect);
}

bool Bloodlust::Graphics::fillRect(const SDL_FRect* rect) {
	return SDL_RenderFillRect(app->renderer, rect);
}

bool Bloodlust::Graphics::fillCircle(const Vector2& origin, float radius) {
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

bool Bloodlust::Graphics::drawTexture(Texture* texture, const Vector2& pos, const Vector2& size) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	return drawTexture(texture, &drawRect);
}

bool Bloodlust::Graphics::drawTexture(
	Texture* texture,
	const Vector2& pos,
	const Vector2& size,
	const Vector2& clipPos,
	const Vector2& clipSize
) {
	SDL_FRect drawRect = {pos.x, pos.y, size.x, size.y};
	SDL_FRect clipRect = {clipPos.x, clipPos.y, clipSize.x, clipSize.y};
	return drawTexture(texture, &drawRect, &clipRect);
}

bool Bloodlust::Graphics::drawTexture(Texture* texture, const SDL_FRect* rect) {
	return SDL_RenderTexture(app->renderer, texture->getData(), rect, nullptr);
}

bool Bloodlust::Graphics::drawTexture(
	Texture* texture,
	const SDL_FRect* rect,
	const SDL_FRect* clipRect
) {
	return SDL_RenderTexture(app->renderer, texture->getData(), rect, clipRect);
}

bool Bloodlust::Graphics::setPixel(const Vector2& pos) {
	return SDL_RenderPoint(app->renderer, pos.x, pos.y);
}

bool Bloodlust::Graphics::setDrawBlendMode(BlendMode mode) {
	SDL_BlendMode blend;
	switch (mode) {
		case BlendMode::NONE: blend = SDL_BLENDMODE_NONE; break;
		case BlendMode::BLEND: blend = SDL_BLENDMODE_BLEND; break;
		case BlendMode::BLEND_PREMUL: blend = SDL_BLENDMODE_BLEND_PREMULTIPLIED; break;
		case BlendMode::ADD: blend = SDL_BLENDMODE_ADD; break;
		case BlendMode::ADD_PREMUL: blend = SDL_BLENDMODE_ADD_PREMULTIPLIED; break;
		case BlendMode::MODULATE: blend = SDL_BLENDMODE_MOD; break;
		case BlendMode::MULTIPLY: blend = SDL_BLENDMODE_MUL; break;
		default: SDL_SetError("Invalid blend mode"); return false;
	}
	return SDL_SetRenderDrawBlendMode(app->renderer, blend);
}

void Bloodlust::Graphics::setDrawColor(uint32_t color) {
	SDL_SetRenderDrawColor(
		app->renderer,
		uint8_t(color & 0xFF),
		uint8_t(color & 0xFF00),
		uint8_t(color & 0xFF0000),
		uint8_t(color & 0xFF00000)
	);
}

void Bloodlust::Graphics::setDrawColor(const Color& color) {
	SDL_SetRenderDrawColor(app->renderer, color.r, color.g, color.b, color.a);
}

void Bloodlust::Graphics::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	SDL_SetRenderDrawColor(app->renderer, r, g, b, a);
}

void Bloodlust::Graphics::setDrawColor(float r, float g, float b, float a) {
	SDL_SetRenderDrawColorFloat(app->renderer, r, g, b, a);
}
