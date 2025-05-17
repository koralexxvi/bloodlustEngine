#ifndef BL_DATA_SOUNDCLIP_H
#define BL_DATA_SOUNDCLIP_H

#include "baseSound.hpp"

#include <SDL3_mixer/SDL_mixer.h>

namespace Bloodlust::DataTypes {

class SoundClip : public BaseSound {
  private:
	Mix_Chunk* sfxData;
	int channel;
	float volume = 1.0f;
	float volumeLeft = 1.0f;
	float volumeRight = 1.0f;
	float angle = 0.0f;
	uint8_t distance = 0;

	[[maybe_unused]]
	char _padding[3];

  public:
	SoundClip(Mix_Chunk* data, int channel = -1, int loopCount = 0);

	void play() override;
	void pause() const override;
	void stop() const override;
	bool isPlaying() const override;

	float getVolume() const override;
	void setVolume(float volume) override;

	void getPanning(float* left, float* right) const;
	void setPanning(float left, float right);

	float getAngle() const;
	uint8_t getDistance() const;
	void setPosition(float angle, uint8_t distance);

	~SoundClip();

  private:
	void setVolumeInternal();
	void setPanningInternal();
	void setPositionInternal();
};

}  // namespace Bloodlust::DataTypes

#endif
