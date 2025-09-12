#include "../../include/datatype/soundClip.hpp"

#include "../../include/app.hpp"
#include "../../include/audio.hpp"

#include <SDL3/SDL_stdinc.h>
#include <cstdint>

using Bloodlust::SoundClip;

SoundClip::SoundClip(const SDL_AudioSpec& spec, std::uint8_t* sfxData, std::uint32_t length)
	: format(spec),
	  sfxData(sfxData),
	  length(length) {}

bool SoundClip::play() {
	return app->audioState.channels[channel]->playSound(this);
}

void SoundClip::pause() const {}

void SoundClip::stop() const {
	if (isPlaying()) {
		stopChannel(channel);
	}
}

bool SoundClip::isPlaying() const {
	return app->audioState.channels[channel]->getPlayingSound() == this;
}

void SoundClip::setVolume(float newVolume) {
	volume = newVolume;
	if (isPlaying()) {
		app->audioState.channels[channel]->updateVolume();
	}
}

void SoundClip::setPanning(float newPanning) {
	panning = newPanning;
}

void SoundClip::setChannel(std::uint32_t newChannel) {
	channel = newChannel;
}

void SoundClip::setLoopCount(std::int32_t newCount) {
	loopCount = newCount;
}

SoundClip::~SoundClip() {
	stop();
	SDL_free(sfxData);
}
