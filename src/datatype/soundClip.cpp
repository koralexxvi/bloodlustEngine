#include "../../include/datatype/soundClip.hpp"

#include "../../include/app.hpp"
#include "../../include/audio.hpp"

#include <SDL3/SDL_stdinc.h>
#include <cstdint>

using Bloodlust::SoundClip;

SoundClip::SoundClip(
	const SDL_AudioSpec& spec,
	std::uint8_t* sfxData,
	std::uint32_t length
) noexcept
	: format(spec),
	  sfxData(sfxData),
	  length(length) {}

bool SoundClip::play() noexcept {
	return app->audioState.channels[channel]->playSound(this);
}

void SoundClip::pause() const noexcept {}

void SoundClip::stop() const noexcept {
	if (isPlaying()) {
		stopChannel(channel);
	}
}

bool SoundClip::isPlaying() const noexcept {
	return app->audioState.channels[channel]->getPlayingSound() == this;
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
