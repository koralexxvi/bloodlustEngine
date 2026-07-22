#pragma once

#include <SDL3/SDL_audio.h>
#include <bloodlustEngine/asset/soundData.hpp>
#include <cstdint>
#include <string_view>

namespace Bloodlust {

// A sound clip that can be played back.
class SoundClip {
  private:
	// The sound data that this clip holds.
	SoundData sfxData;

	// The sound channel that this clip is assigned to.
	std::uint32_t channel = 0;

	// The volume of this sound. 1.0f represents full volume and 0.0f represents silence. Can
	// exceed 1.0f.
	float volume = 1.0f;

	// The panning of this sound clip. 0.0f represents no panning (center), -1.0f represents left
	// only and 1.0f represents right only.
	float panning = 0.0f;

	// The amount of times that this sound can loop. If set to 0, the sound will not loop, and
	// anything lower will cause the sound to loop infinitely.
	std::int32_t loopCount = 0;

  public:
	// Constructs a sound clip from a format, sample data and length in samples.
	SoundClip(std::string_view path);

	// Destroys this sound clip. The pointer containing the sample data will no longer be valid.
	~SoundClip() noexcept;

	// Plays this sound. Will throw InternalError on failure.
	void play();

	// Pauses this sound, keeping its position until it is played again.
	void pause() const noexcept;

	// Stops this sound. A future call to play() will start from the beginning.
	void stop() const noexcept;

	// Returns true if this sound is currently playing.
	bool isPlaying() const noexcept;

	// Returns the sound data of this sound clip.
	constexpr const SoundData& getData() const noexcept {
		return sfxData;
	}

	// Sets the sound data of this sound clip.
	inline void setData(SoundData&& data) {
		sfxData = std::move(data);
	}

	// Returns the length of this sound, in samples.
	constexpr std::size_t getLength() const noexcept {
		return sfxData.getLength();
	}

	// Returns the volume of this sound. 0.0f represents silence and 1.0f represents full volume.
	// Can exceed 1.0f.
	constexpr float getVolume() const noexcept {
		return volume;
	}

	// Sets the volume of this sound. 0.0f represents silence and 1.0f represents full volume.
	// Can exceed 1.0f.
	void setVolume(float newVolume) noexcept;

	// Returns the panning value of the sound. 0.0f represents no panning (center), -1.0f represents
	// left only and 1.0f represents right only.
	constexpr float getPanning() const noexcept {
		return panning;
	}

	// Sets the panning value of the sound. 0.0f represents no panning (center), -1.0f represents
	// left only and 1.0f represents right only.
	inline void setPanning(float newPanning) noexcept {
		panning = newPanning;
	}

	// Returns the channel this sound is bound to.
	constexpr std::uint32_t getChannel() const noexcept {
		return channel;
	}

	// Sets the channel this sound is bound to. If this is greater than or equal to
	// BL_SOUND_CHANNEL_COUNT, the behaviour is undefined.
	inline void setChannel(std::uint32_t newChannel) noexcept {
		channel = newChannel;
	}

	// Returns the amount of times this sound can loop. If set to 0, the sound will not loop, and
	// anything lower will cause the sound to loop infinitely.
	constexpr std::int32_t getLoopCount() const noexcept {
		return loopCount;
	}

	// Sets the amount of times this sound can loop. If set to 0, the sound will not loop, and
	// anything lower will cause the sound to loop infinitely.
	inline void setLoopCount(std::int32_t newCount) noexcept {
		loopCount = newCount;
	}
};

}  // namespace Bloodlust
