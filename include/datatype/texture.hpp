#pragma once

#include "vector2.hpp"

#include <SDL3/SDL_gpu.h>

namespace Bloodlust {

// A class representing a 2D texture.
class Texture {
  private:
	// A pointer to the SDL_GPUTexture that this texture holds.
	SDL_GPUTexture* textureData = nullptr;

	// The width of this texture.
	uint32_t w;

	// The height of this texture.
	uint32_t h;

  public:
	// Constructs a Texture from a pointer to an SDL_GPUTexture, width and height.
	Texture(SDL_GPUTexture* data, uint32_t w, uint32_t h);

	// Returns a pointer to the SDL_GPUTexture this texture holds. This is valid until the texture
	// is destroyed.
	SDL_GPUTexture* getData() const;

	// Returns the width of the texture.
	uint32_t getWidth() const;

	// Returns the height of the texture.
	uint32_t getHeight() const;

	// Returns the size of the texture.
	Vector2 getSize() const;

	// Destroys this texture. The pointer to the SDL_GPUTexture will no longer be valid.
	~Texture();
};

}  // namespace Bloodlust
