#pragma once

#include "vector2.hpp"

#include <SDL3/SDL_gpu.h>

namespace Bloodlust {

class Texture {
  private:
	SDL_GPUTexture* textureData = nullptr;
	uint32_t w;
	uint32_t h;

  public:
	Texture(SDL_GPUTexture* data, uint32_t w, uint32_t h);

	SDL_GPUTexture* getData() const;
	uint32_t getHeight() const;
	uint32_t getWidth() const;
	Vector2 getSize() const;

	~Texture();
};

}  // namespace Bloodlust
