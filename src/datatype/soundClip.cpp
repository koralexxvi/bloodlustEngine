#include <SDL3/SDL_stdinc.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/audio.hpp>
#include <bloodlustEngine/datatype/soundClip.hpp>
#include <cstdint>

using Bloodlust::SoundClip;

SoundClip::SoundClip(
	const SDL_AudioSpec& spec,
	std::uint8_t* sfxData,
	std::uint32_t length
) noexcept
	: sfxData(sfxData),
	  length(length),
	  format(spec) {}

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

SoundClip::~SoundClip() noexcept {
	stop();
	SDL_free(sfxData);
}
