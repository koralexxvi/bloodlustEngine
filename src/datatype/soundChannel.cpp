#include "../../include/datatype/soundChannel.hpp"

#include "../../include/app.hpp"

#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>
#include <memory>

using Bloodlust::SoundChannel;

SoundChannel::SoundChannel(bool& success) {
	SDL_AudioSpec audioSpec = {.format = SDL_AUDIO_F32, .channels = 2, .freq = 48'000};
	stream = SDL_CreateAudioStream(&audioSpec, &audioSpec);
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

	std::unique_ptr<uint8_t[]> buf;
	if (soundLen - position >= BL_SOUND_BUFFER_SIZE) {
		buf = std::unique_ptr<uint8_t[]>(new uint8_t[BL_SOUND_BUFFER_SIZE]);
		SDL_memcpy(buf.get(), playingSound->getData() + position, BL_SOUND_BUFFER_SIZE);
		return SDL_PutAudioStreamData(stream, buf.get(), BL_SOUND_BUFFER_SIZE);
	}

	int len;
	int maxLoops = playingSound->getLoopCount();
	if (maxLoops < 0 || loopCount > maxLoops) {
		len = BL_SOUND_BUFFER_SIZE - position;
		buf = std::unique_ptr<uint8_t[]>(new uint8_t[size_t(len)]);
		SDL_memcpy(buf.get(), playingSound->getData() + position, len);
	} else {
		// int remaining = BL_SOUND_BUFFER_SIZE;
		len = BL_SOUND_BUFFER_SIZE;
		buf = std::unique_ptr<uint8_t[]>(new uint8_t[BL_SOUND_BUFFER_SIZE]);
		// while (remaining > 0) {
		// }
	}
	return SDL_PutAudioStreamData(stream, buf.get(), len);
}

bool SoundChannel::playSound(SoundClip* sound) {
	SDL_assert(sound != nullptr);
	playingSound = sound;
	soundLen = int(sound->getLength());
	position = 0;
	loopCount = 0;
	return true;
}

void SoundChannel::stopChannel() {
	SDL_ClearAudioStream(stream);
	playingSound = nullptr;
}

SoundChannel::~SoundChannel() {
	SDL_DestroyAudioStream(stream);
}
