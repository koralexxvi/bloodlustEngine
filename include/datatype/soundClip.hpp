#pragma once

#include <SDL3/SDL_audio.h>
#include <cstdint>

namespace Bloodlust {

class SoundClip {
  public:
	const SDL_AudioSpec format;

  private:
	uint32_t channel = 0;

	uint8_t* sfxData;
	uint32_t length;
	float volume = 1.0f;
	float panning = 0.0f;
	int loopCount = 0;

  public:
	SoundClip(const SDL_AudioSpec& format, uint8_t* sfxData, uint32_t length);

	bool play();
	void pause() const;
	void stop() const;
	bool isPlaying() const;

	uint8_t* getData() const;
	uint32_t getLength() const;

	float getVolume() const;
	void setVolume(float newVolume);

	float getPanning() const;
	void setPanning(float pan);

	uint32_t getChannel() const;
	void setChannel(uint32_t newChannel);

	int getLoopCount() const;
	void setLoopCount(int newCount);

	~SoundClip();
};

}  // namespace Bloodlust
