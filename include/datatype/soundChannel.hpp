#pragma once

#include "../util.hpp"
#include "soundClip.hpp"

#include <SDL3/SDL_audio.h>

#ifndef BL_SOUND_BUFFER_SIZE
	#define BL_SOUND_BUFFER_SIZE (512)
#endif

namespace Bloodlust {

class SoundChannel {
  private:
	SoundClip* playingSound = nullptr;
	SDL_AudioStream* stream = nullptr;
	float volume = 1.0f;
	int position = 0;
	int soundLen;
	int loopCount;

  public:
	SoundChannel(bool& success);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool update();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool playSound(SoundClip* sound);

	SoundClip* getPlayingSound() const;

	void stopChannel();

	float getVolume() const;
	void setVolume(float newVolume);

	~SoundChannel();
};

}  // namespace Bloodlust
