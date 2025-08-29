#pragma once

#include <SDL3/SDL_audio.h>
#include <cstdint>

namespace Bloodlust {

// A sound clip thaat can be played back.
class SoundClip {
  public:
	// The format of this sound clip.
	const SDL_AudioSpec format;

  private:
	// The sound channel that this clip is assigned to.
	uint32_t channel = 0;

	// The internal sample data that this clip holds.
	uint8_t* sfxData;

	// The length of this sound, in samples.
	uint32_t length;

	// The volume of this sound. 1.0f represents full volume and 0.0f represents silence. Can
	// exceed 1.0f.
	float volume = 1.0f;

	// The panning of this sound clip. 0.0f represents no panning (center), -1.0f represents left
	// only and 1.0f represents right only.
	float panning = 0.0f;

	// The amount of times that this sound can loop. If set to 0, the sound will not loop, and
	// anything lower will cause the sound to loop infinitely.
	int32_t loopCount = 0;

  public:
	// Constructs a sound clip from a format, sample data and length in samples.
	SoundClip(const SDL_AudioSpec& format, uint8_t* sfxData, uint32_t length);

	// Plays this sound.
	bool play();

	// Pauses this sound, keeping its position until it is played again.
	void pause() const;

	// Stops this sound. A future call to play() will start from the beginning.
	void stop() const;

	// Returns true if this sound is currently playing.
	bool isPlaying() const;

	// Returns the internal sample data of this sound.
	uint8_t* getData() const;

	// Returns the length of this sound, in samples.
	uint32_t getLength() const;

	// Returns the volume of this sound. 0.0f represents silence and 1.0f represents full volume.
	// Can exceed 1.0f.
	float getVolume() const;

	// Sets the volume of this sound. 0.0f represents silence and 1.0f represents full volume.
	// Can exceed 1.0f.
	void setVolume(float newVolume);

	// Returns the panning value of the sound. 0.0f represents no panning (center), -1.0f represents
	// left only and 1.0f represents right only.
	float getPanning() const;

	// Sets the panning value of the sound. 0.0f represents no panning (center), -1.0f represents
	// left only and 1.0f represents right only.
	void setPanning(float pan);

	// Returns the channel this sound is bound to.
	uint32_t getChannel() const;

	// Sets the channel this sound is bound to. If this is greater than or equal to
	// BL_AUDIO_STREAM_COUNT, the behaviour is undefined.
	void setChannel(uint32_t newChannel);

	// Returns the amount of times this sound can loop. If set to 0, the sound will not loop, and
	// anything lower will cause the sound to loop infinitely.
	int32_t getLoopCount() const;

	// Sets the amount of times this sound can loop. If set to 0, the sound will not loop, and
	// anything lower will cause the sound to loop infinitely.
	void setLoopCount(int32_t newCount);

	// Destroys this sound clip. The pointer containing the sample data will no longer be valid.
	~SoundClip();
};

}  // namespace Bloodlust
