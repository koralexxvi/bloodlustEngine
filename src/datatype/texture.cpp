#include "../../include/datatype/texture.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

using Bloodlust::DataTypes::Texture, Bloodlust::DataTypes::TextureFilterType;
using Bloodlust::DataTypes::Vector2;

SDL_ScaleMode Texture::defaultScaleMode = SDL_SCALEMODE_LINEAR;

static SDL_ScaleMode enumToSDLScaleMode(TextureFilterType filterType) {
	switch (filterType) {
		case TextureFilterType::NEAREST: return SDL_SCALEMODE_NEAREST;
		case TextureFilterType::LINEAR: return SDL_SCALEMODE_LINEAR;
		default: return SDL_SCALEMODE_INVALID;
	}
}

void Texture::setDefaultFilterType(TextureFilterType type) {
	defaultScaleMode = enumToSDLScaleMode(type);
}

Texture::Texture(SDL_Texture* data) : textureData(data) {
	SDL_SetTextureScaleMode(textureData, defaultScaleMode);
}

int Texture::getWidth() const {
	return textureData->w;
}

int Texture::getHeight() const {
	return textureData->h;
}

Vector2 Texture::getSize() const {
	return {float(textureData->w), float(textureData->h)};
}

void Texture::setColor(const Color& color) {
	SDL_SetTextureColorMod(textureData, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(textureData, color.a);
}

Texture::~Texture() {
	SDL_DestroyTexture(textureData);
	textureData = nullptr;
}
