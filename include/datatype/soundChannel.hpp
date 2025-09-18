#pragma once

#include "../util.hpp"
#include "soundClip.hpp"

#include <SDL3/SDL_audio.h>
#include <array>

#ifndef BL_SOUND_BUFFER_SIZE
	#define BL_SOUND_BUFFER_SIZE (8192)
#endif

namespace Bloodlust {

// A class that represents a sound channel.
class SoundChannel {
  private:
	using SoundBuffer = std::array<std::uint8_t, BL_SOUND_BUFFER_SIZE>;

	// A pointer to the currently playing sound.
	const SoundClip* playingSound = nullptr;

	// A pointer to the internal audio stream.
	SDL_AudioStream* stream = nullptr;

	// The volume of this audio channel. 0.0f represents silence, while 1.0f represents full volume.
	// Can exceed 1.0f.
	float volume = 1.0f;

	// The position of the playing sound clip, or 0 if not playing anything.
	std::uint32_t position = 0;

	// The length of the currently playing sound clip.
	std::uint32_t soundLen;

	// The loop count of the currently playing sound clip. If set to 0, the sound will not loop, and
	// anything lower will cause the sound to loop infinitely.
	std::int32_t loopCount;

	// Fills the sound buffer with data from the currently playing sound, and returns the amount of
	// bytes written.
	std::uint32_t fillSoundBuffer(SoundBuffer& buf, std::uint32_t bufPos);

  public:
	// Constructs a new SoundChannel and returns the success value.
	SoundChannel(bool& success);

	// Updates the sound channel and returns true on success.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool update();

	// Plays a sound, and returns true on success.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	bool playSound(const SoundClip* sound);

	// Returns a pointer to the currently playing sound, or nullptr if there isn't one.
	constexpr const SoundClip* getPlayingSound() const {
		return playingSound;
	}

	// Stops this sound channel.
	void stopChannel();

	// Returns the volume of this sound channel.
	constexpr float getVolume() const {
		return volume;
	}

	// Sets the volume of this sound channel.
	inline void setVolume(float newVolume) {
		volume = newVolume;
		updateVolume();
	}

	// Updates the volume of the currently playing sound clip. This is called internally when needed
	// and should not be called from application code.
	void updateVolume();

	// Destroys this sound channel. The internal audio stream will be destroyed.
	~SoundChannel();
};

}  // namespace Bloodlust
