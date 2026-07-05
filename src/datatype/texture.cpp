#include <SDL3/SDL_gpu.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/datatype/texture.hpp>

using Bloodlust::Texture;

Texture::Texture(SDL_GPUTexture* data, std::uint32_t w, std::uint32_t h) noexcept
	: textureData(data),
	  w(w),
	  h(h) {}

Texture::~Texture() noexcept {
	SDL_ReleaseGPUTexture(app->renderState.device, textureData);
}
