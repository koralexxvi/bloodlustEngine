#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>
#include <bloodlustEngine/asset/soundData.hpp>
#include <bloodlustEngine/assets.hpp>
#include <bloodlustEngine/exception/assetLoadException.hpp>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>
#include <memory>
#include <string_view>

Bloodlust::SoundData::SoundData(std::string_view path) {
	SDL_IOStream* stream;
	std::uint64_t dataSize;
	std::unique_ptr<std::uint8_t[]> assetData = getAsset(path, dataSize);
	stream = SDL_IOFromConstMem(assetData.get(), dataSize);
	if (stream == nullptr) {
		throw InternalError("Could not create stream from sound data: " + getError());
	}
	if (!SDL_LoadWAV_IO(stream, true, &format, &data, &length)) {
		throw AssetLoadException("Could not load sound clip: " + getError(), path);
	}
}

Bloodlust::SoundData::~SoundData() {
	SDL_free(data);
}
