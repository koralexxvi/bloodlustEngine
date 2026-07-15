#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/asset/soundClip.hpp>
#include <bloodlustEngine/datatype/soundChannel.hpp>
#include <bloodlustEngine/exceptions/internalError.hpp>
#include <bloodlustEngine/logging.hpp>

using Bloodlust::SoundChannel;

std::uint32_t SoundChannel::fillSoundBuffer(SoundBuffer& buf, std::uint32_t bufPos) noexcept {
	std::uint32_t remaining = soundLen - position;
	if (remaining > BL_SOUND_BUFFER_SIZE - bufPos) {
		remaining = BL_SOUND_BUFFER_SIZE - bufPos;
	}
	SDL_memcpy(buf.data() + bufPos, playingSound->getData() + position, remaining);
	position += remaining;
	return remaining;
}

SoundChannel::SoundChannel() {
	stream = SDL_CreateAudioStream(nullptr, nullptr);
	if (stream == nullptr) {
		throw InternalError("Could not create audio stream: " + getError());
	} else if (!SDL_BindAudioStream(app->audioState.deviceOut, stream)) {
		throw InternalError("Could not bind audio stream: " + getError());
	}
}

void SoundChannel::update() {
	if (playingSound == nullptr) {
		return;
	}

	int queued = SDL_GetAudioStreamQueued(stream);
	if (queued == -1) {
		throw InternalError("Failed to get queued audio stream data: " + getError());
	} else if (queued >= BL_SOUND_BUFFER_SIZE) {
		return;
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
				if (!SDL_PutAudioStreamData(
						stream,
						buf.data(),
						static_cast<int>(BL_SOUND_BUFFER_SIZE - bufPos)
					))
				{
					throw InternalError("Failed to put audio stream data: " + getError());
				}
			}
		}
	} while (bufPos < BL_SOUND_BUFFER_SIZE);
	if (!SDL_PutAudioStreamData(stream, buf.data(), BL_SOUND_BUFFER_SIZE)) {
		throw InternalError("Failed to put audio stream data: " + getError());
	}
}

void SoundChannel::playSound(const SoundClip* sound) {
	SDL_assert(sound != nullptr);
	SDL_SetAudioStreamFormat(stream, &sound->format, nullptr);
	playingSound = sound;
	soundLen = sound->getLength();
	position = 0;
	loopCount = 0;
}

void SoundChannel::stopChannel() noexcept {
	SDL_ClearAudioStream(stream);
	playingSound = nullptr;
}

void SoundChannel::updateVolume() noexcept {
	if (playingSound != nullptr) {
		SDL_SetAudioStreamGain(stream, playingSound->getVolume() * volume);
	}
}

SoundChannel::~SoundChannel() noexcept {
	SDL_DestroyAudioStream(stream);
}
