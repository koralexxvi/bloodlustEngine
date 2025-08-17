#include "../../include/datatype/soundClip.hpp"

#include <SDL3/SDL_stdinc.h>
#include <cstdint>

using Bloodlust::SoundClip;

SoundClip::SoundClip(uint8_t* sfxData, uint32_t length) : sfxData(sfxData), length(length) {}

void SoundClip::play() {}

void SoundClip::pause() const {}

void SoundClip::stop() const {}

bool SoundClip::isPlaying() const {
	return false;
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
}

float SoundClip::getPanning() const {
	return panning;
}

void SoundClip::setPanning(float newPanning) {
	panning = newPanning;
}

int SoundClip::getChannel() const {
	return channel;
}

void SoundClip::setChannel(int newChannel) {
	channel = newChannel;
}

int SoundClip::getLoopCount() const {
	return loopCount;
}

void SoundClip::setLoopCount(int newCount) {
	loopCount = newCount;
}

SoundClip::~SoundClip() {
	SDL_free(sfxData);
}
