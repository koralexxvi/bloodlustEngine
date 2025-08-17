#include "../../include/datatype/soundChannel.hpp"

#include "../../include/app.hpp"

#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>
#include <memory>

using Bloodlust::SoundChannel;

SoundChannel::SoundChannel(bool& success) {
	stream = SDL_CreateAudioStream(nullptr, nullptr);
	if (stream == nullptr) {
		success = false;
		return;
	}
	success = SDL_BindAudioStream(app->audioState.deviceOut, stream);
}

bool SoundChannel::update() {
	constexpr size_t sampleSize = BL_SOUND_BUFFER_SIZE * sizeof(float) * 2;
	if (playingSound == nullptr) {
		return true;
	}

	int queued = SDL_GetAudioStreamQueued(stream);
	if (queued == -1) {
		return false;
	} else if (size_t(queued) >= sampleSize) {
		return true;
	}

	std::unique_ptr<float[]> buf;
	if (soundLen - position >= int(sampleSize)) {
		buf = std::unique_ptr<float[]>(new float[sampleSize / sizeof(float)]);
		SDL_memcpy(buf.get(), playingSound->getData() + position, sampleSize);
		position += sampleSize;
		return SDL_PutAudioStreamData(stream, buf.get(), sampleSize);
	}

	return true;
}

bool SoundChannel::playSound(SoundClip* sound) {
	SDL_assert(sound != nullptr);
	SDL_SetAudioStreamFormat(stream, &sound->format, nullptr);
	playingSound = sound;
	soundLen = int(sound->getLength());
	position = 0;
	loopCount = 0;
	return true;
}

Bloodlust::SoundClip* SoundChannel::getPlayingSound() const {
	return playingSound;
}

void SoundChannel::stopChannel() {
	SDL_ClearAudioStream(stream);
	playingSound = nullptr;
}

float SoundChannel::getVolume() const {
	return volume;
}

void SoundChannel::setVolume(float newVolume) {
	volume = newVolume;
	SDL_SetAudioStreamGain(stream, newVolume);
}

SoundChannel::~SoundChannel() {
	SDL_DestroyAudioStream(stream);
}
