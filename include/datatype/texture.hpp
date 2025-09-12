#pragma once

#include "vector2.hpp"

#include <SDL3/SDL_gpu.h>
#include <cstdint>

namespace Bloodlust {

// A class representing a 2D texture.
class Texture {
  private:
	// A pointer to the SDL_GPUTexture that this texture holds.
	SDL_GPUTexture* textureData = nullptr;

	// The width of this texture.
	std::uint32_t w;

	// The height of this texture.
	std::uint32_t h;

  public:
	// Constructs a Texture from a pointer to an SDL_GPUTexture, width and height.
	Texture(SDL_GPUTexture* data, std::uint32_t w, std::uint32_t h);

	// Returns a pointer to the SDL_GPUTexture this texture holds. This is valid until the texture
	// is destroyed.
	constexpr SDL_GPUTexture* getData() const {
		return textureData;
	}

	// Returns the width of the texture.
	constexpr std::uint32_t getWidth() const {
		return w;
	}

	// Returns the height of the texture.
	constexpr std::uint32_t getHeight() const {
		return h;
	}

	// Returns the size of the texture.
	constexpr Vector2 getSize() const {
		return {static_cast<float>(w), static_cast<float>(h)};
	}

	// Destroys this texture. The pointer to the SDL_GPUTexture will no longer be valid.
	~Texture();
};

}  // namespace Bloodlust
