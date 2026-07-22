#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_storage.h>
#include <SDL3/SDL_timer.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/asset/texture.hpp>
#include <bloodlustEngine/assets.hpp>
#include <bloodlustEngine/exception/assetLoadException.hpp>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>
#include <cstdint>
#include <memory>
#include <stb/stb_image.h>
#include <unordered_map>

namespace {

std::unordered_map<std::string, std::unique_ptr<Bloodlust::Texture>> loadedTextures = {};

}

std::unique_ptr<std::uint8_t[]> Bloodlust::getAsset(std::string_view path, std::uint64_t& sizeOut) {
	SDL_Storage* title = SDL_OpenTitleStorage(nullptr, 0);
	if (title == nullptr) {
		throw AssetLoadException("Could not open title storage: " + getError());
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
				throw AssetLoadException(getError(), path);
			}
		} else {
			throw AssetLoadException("File is empty", path);
		}
	} else {
		throw AssetLoadException(getError(), path);
	}
	SDL_CloseStorage(title);
	sizeOut = size;
	return std::unique_ptr<std::uint8_t[]>(data);
}

Bloodlust::Texture& Bloodlust::loadTexture(std::string_view path) {
	std::string pathStr = path.data();
	if (loadedTextures.contains(pathStr)) {
		return *loadedTextures[pathStr].get();
	}

	int iw, ih, nChannels;
	std::uint8_t* imgData = stbi_load(pathStr.c_str(), &iw, &ih, &nChannels, 0);
	if (imgData == nullptr) {
		throw AssetLoadException("Could not load texture", path);
	}

	std::uint32_t w = static_cast<std::uint32_t>(iw), h = static_cast<std::uint32_t>(ih);
	std::unique_ptr<Texture> texture = std::make_unique<Texture>(Texture::CreateParams{w, h});

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
	destination.texture = texture->getData();
	destination.w = w;
	destination.h = h;
	destination.d = 1;
	SDL_UploadToGPUTexture(copyPass, &source, &destination, false);
	SDL_EndGPUCopyPass(copyPass);

	bool submitSuccess = SDL_SubmitGPUCommandBuffer(cmdbuf);
	SDL_assert(submitSuccess);
	SDL_ReleaseGPUTransferBuffer(app->renderState.device, transBuf);

	loadedTextures[pathStr] = std::move(texture);
	return *loadedTextures[pathStr].get();
}

void Bloodlust::unloadTexture(std::string_view path) noexcept {
	loadedTextures.erase(std::string(path).c_str());
}

std::unique_ptr<Bloodlust::SoundClip> Bloodlust::loadSoundClip(std::string_view path) {
	// TODO: maybe split sound data and sound clips, then cache the data?
	return std::make_unique<Bloodlust::SoundClip>(path);
}

void Bloodlust::unloadAssets() noexcept {
	loadedTextures.clear();
}
