#include "../include/assets.hpp"

#include "../extern/stb_image.h"
#include "../include/app.hpp"
#include "../include/logging.hpp"

#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_storage.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cstdint>
#include <memory>

// TODO: error messages

std::unique_ptr<uint8_t[]> Bloodlust::getAsset(const std::string& path, uint64_t& sizeOut) {
	SDL_Storage* title = SDL_OpenTitleStorage(nullptr, 0);
	if (title == nullptr) {
		return nullptr;
	}
	while (!SDL_StorageReady(title)) {
		SDL_Delay(1);
	}

	uint8_t* data = nullptr;
	uint64_t size = 0;
	if (SDL_GetStorageFileSize(title, path.c_str(), &size)) {
		if (size > 0) {
			data = new uint8_t[size];
			if (!SDL_ReadStorageFile(title, path.c_str(), data, size)) {
				delete[] data;
				data = nullptr;
			}
		} else {
			setError("File is empty");
		}
	}

	SDL_CloseStorage(title);
	if (data == nullptr) {
		return nullptr;
	}

	sizeOut = size;
	return std::unique_ptr<uint8_t[]>(data);
}

Bloodlust::Texture* Bloodlust::loadTexture(const char* path) {
	SDL_GPUTransferBuffer* transBuf = nullptr;
	SDL_GPUCommandBuffer* cmdbuf = nullptr;
	SDL_GPUCopyPass* copyPass = nullptr;
	uint8_t* texDataPtr;
	SDL_GPUTextureTransferInfo source = {};
	SDL_GPUTextureRegion destination = {};

	int iw, ih, nChannels;
	uint8_t* imgData = stbi_load(path, &iw, &ih, &nChannels, 0);
	if (imgData == nullptr) {
		return nullptr;
	}
	uint32_t w = uint32_t(iw), h = uint32_t(ih);

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
	if (texture == nullptr) {
		goto fail;
	}

	SDL_GPUTransferBufferCreateInfo transBufInfo;
	transBufInfo = {.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, .size = w * h * 4, .props = 0};
	transBuf = SDL_CreateGPUTransferBuffer(app->renderState.device, &transBufInfo);
	if (transBuf == nullptr) {
		goto fail;
	}
	texDataPtr = (uint8_t*) SDL_MapGPUTransferBuffer(app->renderState.device, transBuf, false);
	SDL_memcpy(texDataPtr, imgData, w * h * 4);
	SDL_UnmapGPUTransferBuffer(app->renderState.device, transBuf);
	stbi_image_free(imgData);

	cmdbuf = SDL_AcquireGPUCommandBuffer(app->renderState.device);
	if (cmdbuf == nullptr) {
		goto fail;
	}
	copyPass = SDL_BeginGPUCopyPass(cmdbuf);
	source.transfer_buffer = transBuf;
	destination.texture = texture;
	destination.w = w;
	destination.h = h;
	destination.d = 1;
	SDL_UploadToGPUTexture(copyPass, &source, &destination, false);
	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(cmdbuf);
	SDL_ReleaseGPUTransferBuffer(app->renderState.device, transBuf);
	return new Bloodlust::Texture(texture, w, h);

fail:
	if (cmdbuf != nullptr) {
		SDL_CancelGPUCommandBuffer(cmdbuf);
	}
	stbi_image_free(imgData);
	SDL_ReleaseGPUTexture(app->renderState.device, texture);
	SDL_ReleaseGPUTransferBuffer(app->renderState.device, transBuf);
	return nullptr;
}

Bloodlust::SoundClip* Bloodlust::loadSoundClip(const char* path) {
	uint64_t dataSize;
	std::unique_ptr<uint8_t[]> data = getAsset(path, dataSize);
	if (data == nullptr) {
		return nullptr;
	}
	SDL_IOStream* stream = SDL_IOFromConstMem(data.get(), dataSize);
	if (stream != nullptr) {
		Mix_Chunk* chunk = Mix_LoadWAV_IO(stream, true);
		if (chunk != nullptr) {
			return new SoundClip(chunk);
		}
	}
	return nullptr;
}

Bloodlust::MusicTrack* Bloodlust::loadMusicTrack(const char* path) {
	uint64_t dataSize;
	std::unique_ptr<uint8_t[]> data = getAsset(path, dataSize);
	if (data == nullptr) {
		return nullptr;
	}
	SDL_IOStream* stream = SDL_IOFromConstMem(data.get(), dataSize);
	if (stream != nullptr) {
		Mix_Music* musicTrack = Mix_LoadMUS_IO(stream, true);
		if (musicTrack != nullptr) {
			return new MusicTrack(musicTrack);
		}
	}
	return nullptr;
}
