#include <SDL3/SDL_stdinc.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/asset/soundClip.hpp>
#include <bloodlustEngine/assets.hpp>
#include <bloodlustEngine/audio.hpp>
#include <bloodlustEngine/exception/assetLoadException.hpp>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>
#include <cstdint>
#include <string_view>

using Bloodlust::SoundClip;

SoundClip::SoundClip(std::string_view path) {
	SDL_IOStream* stream;
	std::uint64_t dataSize;
	std::unique_ptr<std::uint8_t[]> data = getAsset(path, dataSize);
	stream = SDL_IOFromConstMem(data.get(), dataSize);
	if (stream == nullptr) {
		throw InternalError("Could not create stream from sound data: " + getError());
	}
	if (!SDL_LoadWAV_IO(stream, true, &format, &sfxData, &length)) {
		throw AssetLoadException("Could not load sound clip: " + getError(), path);
	}
}

SoundClip::~SoundClip() noexcept {
	stop();
	SDL_free(sfxData);
}

void SoundClip::play() {
	if (isAudioDeviceOpen()) {
		app->audioState.channels[channel]->playSound(this);
	}
}

void SoundClip::pause() const noexcept {}

void SoundClip::stop() const noexcept {
	if (isAudioDeviceOpen() && isPlaying()) {
		stopChannel(channel);
	}
}

bool SoundClip::isPlaying() const noexcept {
	return isAudioDeviceOpen() && app->audioState.channels[channel]->getPlayingSound() == this;
}

void SoundClip::setVolume(float newVolume) noexcept {
	volume = newVolume;
	if (isPlaying()) {
		app->audioState.channels[channel]->updateVolume();
	}
}
