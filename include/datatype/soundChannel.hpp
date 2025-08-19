#pragma once

#include "../util.hpp"
#include "soundClip.hpp"

#include <SDL3/SDL_audio.h>
#include <array>

#ifndef BL_SOUND_BUFFER_SIZE
	#define BL_SOUND_BUFFER_SIZE (8192)
#endif

namespace Bloodlust {

class SoundChannel {
  private:
	using SoundBuffer = std::array<uint8_t, BL_SOUND_BUFFER_SIZE>;

	const SoundClip* playingSound = nullptr;
	SDL_AudioStream* stream = nullptr;
	float volume = 1.0f;
	uint32_t position = 0;
	uint32_t soundLen;
	int loopCount;

	uint32_t fillSoundBuffer(SoundBuffer& buf, uint32_t bufPos);

  public:
	SoundChannel(bool& success);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool update();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool playSound(const SoundClip* sound);

	const SoundClip* getPlayingSound() const;

	void stopChannel();

	float getVolume() const;
	void setVolume(float newVolume);

	void updateVolume();

	~SoundChannel();
};

}  // namespace Bloodlust
