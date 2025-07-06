#include "../../include/datatype/texture.hpp"

#include "../../include/app.hpp"

#include <SDL3/SDL_gpu.h>

using Bloodlust::Texture;

Texture::Texture(SDL_GPUTexture* data, uint32_t w, uint32_t h) : textureData(data), w(w), h(h) {}

SDL_GPUTexture* Texture::getData() const {
	return textureData;
}

uint32_t Texture::getWidth() const {
	return w;
}

uint32_t Texture::getHeight() const {
	return h;
}

Bloodlust:: Vector2 Texture::getSize() const {
	return {float(w), float(h)};
}

Texture::~Texture() {
	SDL_ReleaseGPUTexture(app->renderState.device, textureData);
}
