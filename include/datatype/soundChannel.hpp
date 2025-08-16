#pragma once

#include "../util.hpp"
#include "soundClip.hpp"

#include <SDL3/SDL_audio.h>

namespace Bloodlust {

class SoundChannel {
  private:
	SoundClip* playingSound = nullptr;
	SDL_AudioStream* stream = nullptr;

  public:
	SoundChannel();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool update();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool addSound();

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool removeSound();

	bool isSoundBound();

	void stopChannel();

	~SoundChannel();
};

}  // namespace Bloodlust
