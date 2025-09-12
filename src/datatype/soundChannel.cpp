#include "../../include/datatype/soundChannel.hpp"

#include "../../include/app.hpp"

#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>

using Bloodlust::SoundChannel;

std::uint32_t SoundChannel::fillSoundBuffer(SoundBuffer& buf, std::uint32_t bufPos) {
	std::uint32_t remaining = soundLen - position;
	if (remaining > BL_SOUND_BUFFER_SIZE - bufPos) {
		remaining = BL_SOUND_BUFFER_SIZE - bufPos;
	}
	SDL_memcpy(buf.data() + bufPos, playingSound->getData() + position, remaining);
	position += remaining;
	return remaining;
}

SoundChannel::SoundChannel(bool& success) {
	stream = SDL_CreateAudioStream(nullptr, nullptr);
	if (stream == nullptr) {
		success = false;
		return;
	}
	success = SDL_BindAudioStream(app->audioState.deviceOut, stream);
}

bool SoundChannel::update() {
	if (playingSound == nullptr) {
		return true;
	}

	int queued = SDL_GetAudioStreamQueued(stream);
	if (queued == -1) {
		return false;
	} else if (queued >= BL_SOUND_BUFFER_SIZE) {
		return true;
	}

	std::uint32_t bufPos = 0;
	SoundBuffer buf = {};
	do {
		bufPos += fillSoundBuffer(buf, bufPos);
		if (position == soundLen) {
			std::int32_t maxLoops = playingSound->getLoopCount();
			position = 0;
			if (maxLoops < 0) {
				continue;
			}
			loopCount++;
			if (loopCount > maxLoops) {
				playingSound = nullptr;
				return SDL_PutAudioStreamData(
					stream,
					buf.data(),
					static_cast<int>(BL_SOUND_BUFFER_SIZE - bufPos)
				);
			}
		}
	} while (bufPos < BL_SOUND_BUFFER_SIZE);
	return SDL_PutAudioStreamData(stream, buf.data(), BL_SOUND_BUFFER_SIZE);
}

bool SoundChannel::playSound(const SoundClip* sound) {
	SDL_assert(sound != nullptr);
	SDL_SetAudioStreamFormat(stream, &sound->format, nullptr);
	playingSound = sound;
	soundLen = sound->getLength();
	position = 0;
	loopCount = 0;
	return true;
}

void SoundChannel::stopChannel() {
	SDL_ClearAudioStream(stream);
	playingSound = nullptr;
}

void SoundChannel::setVolume(float newVolume) {
	volume = newVolume;
	updateVolume();
}

void SoundChannel::updateVolume() {
	if (playingSound != nullptr) {
		SDL_SetAudioStreamGain(stream, playingSound->getVolume() * volume);
	}
}

SoundChannel::~SoundChannel() {
	SDL_DestroyAudioStream(stream);
}
