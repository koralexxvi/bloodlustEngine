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
	int soundLen;
	int position = 0;
	int loopCount;

	[[maybe_unused]]
	char _padding[4];

  public:
	SoundChannel(bool& success);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool update();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool playSound(SoundClip* sound);

	void stopChannel();

	~SoundChannel();
};

}  // namespace Bloodlust
