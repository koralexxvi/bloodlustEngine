#ifndef BL_DATA_BASESOUND_H
#define BL_DATA_BASESOUND_H

namespace Bloodlust::DataTypes {

struct BaseSound {
	int loopCount;

	virtual void play() = 0;
	virtual void pause() const = 0;
	virtual void stop() const = 0;
	virtual bool isPlaying() const = 0;

	virtual float getVolume() const = 0;
	virtual void setVolume(float volume) = 0;

	virtual ~BaseSound() = default;
};

}  // namespace Bloodlust::DataTypes

#endif
