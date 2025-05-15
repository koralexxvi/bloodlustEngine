#include "../include/assets.hpp"

#include "../include/logging.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_storage.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cstdint>

using namespace Bloodlust;
using namespace DataTypes;

// TODO: error messages

[[nodiscard("returned pointer is not automatically freed")]]
SDL_IOStream* Assets::getAsset(const std::string& path, void** dataOut, uint64_t* sizeOut) {
	SDL_Storage* title = SDL_OpenTitleStorage(nullptr, 0);
	if (title == nullptr) {
		return nullptr;
	}
	while (!SDL_StorageReady(title)) {
		SDL_Delay(1);
	}

	void* data = nullptr;
	uint64_t size = 0;

	if (SDL_GetStorageFileSize(title, path.c_str(), &size)) {
		if (size > 0) {
			data = SDL_malloc(size);
			if (data == nullptr) {
				Logging::setError("Failed to allocate data buffer");
			} else if (!SDL_ReadStorageFile(title, path.c_str(), data, size)) {
				SDL_free(data);
				data = nullptr;
			}
		} else {
			Logging::setError("File is empty");
		}
	}

	SDL_CloseStorage(title);
	if (data == nullptr) {
		return nullptr;
	}

	SDL_IOStream* stream = SDL_IOFromConstMem(data, size);
	if (stream == nullptr) {
		SDL_free(data);
		data = nullptr;
	}
	*dataOut = data;
	if (sizeOut != nullptr) {
		*sizeOut = size;
	}
	return stream;
}

Texture* Assets::loadTexture(SDL_Renderer* renderer, const std::string& path) {
	void* data;
	SDL_IOStream* stream = getAsset(path.c_str(), &data, nullptr);
	if (stream != nullptr) {
		SDL_Texture* textureData = IMG_LoadTexture_IO(renderer, stream, true);
		SDL_free(data);
		if (textureData != nullptr) {
			return new Texture(textureData);
		}
	}
	return nullptr;
}

SoundClip* Assets::loadSoundClip(const std::string& path) {
	void* data;
	SDL_IOStream* stream = getAsset(path.c_str(), &data, nullptr);
	if (stream != nullptr) {
		Mix_Chunk* chunk = Mix_LoadWAV_IO(stream, true);
		SDL_free(data);
		if (chunk != nullptr) {
			return new SoundClip(chunk);
		}
	}
	return nullptr;
}

MusicTrack* Assets::loadMusicTrack(const std::string& path) {
	void* data;
	SDL_IOStream* stream = getAsset(path.c_str(), &data, nullptr);
	if (stream != nullptr) {
		Mix_Music* musicData = Mix_LoadMUS_IO(stream, true);
		SDL_free(data);
		if (musicData != nullptr) {
			return new MusicTrack(musicData);
		}
	}
	return nullptr;
}
