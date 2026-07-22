#include <SDL3/SDL_gpu.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/asset/texture.hpp>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>

using Bloodlust::Texture;

Texture::Texture(const CreateParams& params) : w(params.width), h(params.height) {
	SDL_GPUTextureCreateInfo createInfo = {
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = w,
		.height = h,
		.layer_count_or_depth = params.layers,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	if (createInfo.layer_count_or_depth != 1) {
		createInfo.type = SDL_GPU_TEXTURETYPE_2D_ARRAY;
	}
	textureData = SDL_CreateGPUTexture(app->renderState.device, &createInfo);
	if (textureData == nullptr) {
		throw InternalError("Could not create texture: " + getError());
	}
}

Texture::~Texture() noexcept {
	SDL_ReleaseGPUTexture(app->renderState.device, textureData);
}
