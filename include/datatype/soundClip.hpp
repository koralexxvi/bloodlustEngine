#pragma once

#include <cstdint>

namespace Bloodlust {

class SoundClip {
  private:
	uint8_t* sfxData;
	uint32_t length;
	float volume = 1.0f;
	float panning = 0.0f;
	int channel = 0;
	int loopCount = 0;

	[[maybe_unused]]
	char _padding[4];

  public:
	SoundClip(uint8_t* sfxData, uint32_t length);

	void play();
	void pause() const;
	void stop() const;
	bool isPlaying() const;

	uint8_t* getData() const;
	uint32_t getLength() const;

	float getVolume() const;
	void setVolume(float newVolume);

	float getPanning() const;
	void setPanning(float pan);

	int getChannel() const;
	void setChannel(int newChannel);

	int getLoopCount() const;
	void setLoopCount(int newCount);

	~SoundClip();
};

}  // namespace Bloodlust
