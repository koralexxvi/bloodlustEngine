#include "../../include/datatype/soundClip.hpp"

#include "../../include/app.hpp"
#include "../../include/audio.hpp"

#include <SDL3/SDL_stdinc.h>
#include <cstdint>

using Bloodlust::SoundClip;

SoundClip::SoundClip(const SDL_AudioSpec& spec, uint8_t* sfxData, uint32_t length)
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

uint8_t* SoundClip::getData() const {
	return sfxData;
}

uint32_t SoundClip::getLength() const {
	return length;
}

float SoundClip::getVolume() const {
	return volume;
}

void SoundClip::setVolume(float newVolume) {
	volume = newVolume;
	if (isPlaying()) {
		app->audioState.channels[channel]->updateVolume();
	}
}

float SoundClip::getPanning() const {
	return panning;
}

void SoundClip::setPanning(float newPanning) {
	panning = newPanning;
}

uint32_t SoundClip::getChannel() const {
	return channel;
}

void SoundClip::setChannel(uint32_t newChannel) {
	channel = newChannel;
}

int32_t SoundClip::getLoopCount() const {
	return loopCount;
}

void SoundClip::setLoopCount(int32_t newCount) {
	loopCount = newCount;
}

SoundClip::~SoundClip() {
	stop();
	SDL_free(sfxData);
}
