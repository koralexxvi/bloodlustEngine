#include "../../include/datatype/soundClip.hpp"

#include "../../include/math.hpp"

#include <SDL3_mixer/SDL_mixer.h>

using Bloodlust::SoundClip;

SoundClip::SoundClip(Mix_Chunk* data, int channel, int loopCount)
	: sfxData(data),
	  channel(channel) {
	this->loopCount = loopCount;
}

void SoundClip::play() {
	setVolumeInternal();
	setPanningInternal();
	setPositionInternal();
	Mix_PlayChannel(channel, sfxData, loopCount);
}

void SoundClip::pause() const {
	if (isPlaying()) {
		Mix_Pause(channel);
	}
}

void SoundClip::stop() const {
	if (isPlaying()) {
		Mix_HaltChannel(channel);
	}
}

bool SoundClip::isPlaying() const {
	return Mix_Playing(channel) && Mix_GetChunk(channel) == sfxData && !Mix_Paused(channel);
}

float SoundClip::getVolume() const {
	return volume;
}

void SoundClip::setVolume(float volume) {
	this->volume = volume;
	if (isPlaying()) {
		setVolumeInternal();
	}
}

void SoundClip::getPanning(float* left, float* right) const {
	if (left != nullptr) {
		*left = volumeLeft;
	}
	if (right != nullptr) {
		*right = volumeRight;
	}
}

void SoundClip::setPanning(float left, float right) {
	volumeLeft = left;
	volumeRight = right;
	if (isPlaying()) {
		setPanningInternal();
	}
}

float SoundClip::getAngle() const {
	return angle;
}

uint8_t SoundClip::getDistance() const {
	return distance;
}

void SoundClip::setPosition(float angle, uint8_t distance) {
	this->angle = angle;
	this->distance = distance;
	if (isPlaying()) {
		setPositionInternal();
	}
}

SoundClip::~SoundClip() {
	Mix_FreeChunk(sfxData);
	sfxData = nullptr;
}

void SoundClip::setVolumeInternal() {
	Mix_Volume(channel, int(MIX_MAX_VOLUME * volume));
}

void SoundClip::setPanningInternal() {
	Mix_SetPanning(
		channel,
		uint8_t(MIX_MAX_VOLUME * volumeLeft),
		uint8_t(MIX_MAX_VOLUME * volumeRight)
	);
}

void SoundClip::setPositionInternal() {
	Mix_SetPosition(channel, int16_t(toDegrees(angle)), distance);
}
