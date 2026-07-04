#include "../include/assets.hpp"

#include "../include/app.hpp"
#include "../include/logging.hpp"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_storage.h>
#include <SDL3/SDL_timer.h>
#include <cstdint>
#include <memory>
#include <stb/stb_image.h>
#include <unordered_map>

namespace {

std::unordered_map<std::string, std::unique_ptr<Bloodlust::Texture>> loadedTextures = {};

}

std::unique_ptr<std::uint8_t[]> Bloodlust::getAsset(
	std::string_view path,
	std::uint64_t& sizeOut
) noexcept {
	SDL_Storage* title = SDL_OpenTitleStorage(nullptr, 0);
	if (title == nullptr) {
		setError("Could not open title storage: {}", getError());
		return nullptr;
	}
	while (!SDL_StorageReady(title)) {
		SDL_Delay(1);
	}

	std::uint8_t* data = nullptr;
	std::uint64_t size = 0;
	std::string pathStr = std::string(path);
	if (SDL_GetStorageFileSize(title, pathStr.c_str(), &size)) {
		if (size > 0) {
			data = new std::uint8_t[size];
			if (!SDL_ReadStorageFile(title, pathStr.c_str(), data, size)) {
				delete[] data;
				data = nullptr;
			}
		} else {
			setError("File is empty");
		}
	}

	if (data == nullptr) {
		log("Could not get asset at path {}: {}", pathStr, getError());
		SDL_CloseStorage(title);
		return nullptr;
	}
	SDL_CloseStorage(title);

	sizeOut = size;
	return std::unique_ptr<std::uint8_t[]>(data);
}

Bloodlust::Texture* Bloodlust::loadTexture(std::string_view path) noexcept {
	std::string pathStr = path.data();
	if (loadedTextures.contains(pathStr)) {
		return loadedTextures[pathStr].get();
	}

	int iw, ih, nChannels;
	std::uint8_t* imgData = stbi_load(pathStr.c_str(), &iw, &ih, &nChannels, 0);
	if (imgData == nullptr) {
		setError("Could not load texture at {}", pathStr);
		return nullptr;
	}

	std::uint32_t w = static_cast<std::uint32_t>(iw), h = static_cast<std::uint32_t>(ih);
	SDL_GPUTextureCreateInfo texInfo = {
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = w,
		.height = h,
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	SDL_GPUTexture* texture = SDL_CreateGPUTexture(app->renderState.device, &texInfo);
	SDL_assert(texture != nullptr);

	SDL_GPUTransferBufferCreateInfo transBufInfo =
		{.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = w * h * 4, .props = 0};

	SDL_GPUTransferBuffer* transBuf =
		SDL_CreateGPUTransferBuffer(app->renderState.device, &transBufInfo);
	SDL_assert(transBuf != nullptr);

	std::uint8_t* texDataPtr = static_cast<std::uint8_t*>(
		SDL_MapGPUTransferBuffer(app->renderState.device, transBuf, false)
	);
	SDL_assert(texDataPtr != nullptr);
	SDL_memcpy(texDataPtr, imgData, w * h * 4);
	SDL_UnmapGPUTransferBuffer(app->renderState.device, transBuf);
	stbi_image_free(imgData);
	texDataPtr = nullptr;
	imgData = nullptr;

	SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(app->renderState.device);
	SDL_assert(cmdbuf != nullptr);

	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdbuf);
	SDL_assert(copyPass != nullptr);

	SDL_GPUTextureTransferInfo source = {};
	SDL_GPUTextureRegion destination = {};
	source.transfer_buffer = transBuf;
	destination.texture = texture;
	destination.w = w;
	destination.h = h;
	destination.d = 1;
	SDL_UploadToGPUTexture(copyPass, &source, &destination, false);
	SDL_EndGPUCopyPass(copyPass);

	bool submitSuccess = SDL_SubmitGPUCommandBuffer(cmdbuf);
	SDL_assert(submitSuccess);
	SDL_ReleaseGPUTransferBuffer(app->renderState.device, transBuf);

	loadedTextures[pathStr] = std::make_unique<Bloodlust::Texture>(texture, w, h);
	return loadedTextures[pathStr].get();
}

void Bloodlust::unloadTexture(std::string_view path) noexcept {
	loadedTextures.erase(std::string(path).c_str());
}

std::unique_ptr<Bloodlust::SoundClip> Bloodlust::loadSoundClip(std::string_view path) noexcept {
	SDL_IOStream* stream;
	std::uint64_t dataSize;
	std::unique_ptr<std::uint8_t[]> data = getAsset(path, dataSize);
	if (data == nullptr) {
		goto fail;
	}
	stream = SDL_IOFromConstMem(data.get(), dataSize);
	if (stream != nullptr) {
		std::uint8_t* data;
		std::uint32_t len;
		SDL_AudioSpec spec;
		bool success = SDL_LoadWAV_IO(stream, true, &spec, &data, &len);
		if (success) {
			return std::make_unique<Bloodlust::SoundClip>(spec, data, len);
		}
	}

fail:
	setError("Could not load sound clip: {}", getError());
	return nullptr;
}

void Bloodlust::unloadAssets() noexcept {
	loadedTextures.clear();
}
