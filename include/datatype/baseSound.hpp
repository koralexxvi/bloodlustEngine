#pragma once

namespace Bloodlust {

struct BaseSound {
	int loopCount;

  private:
	[[maybe_unused]]
	char _padding[4];

  public:
	virtual void play() = 0;
	virtual void pause() const = 0;
	virtual void stop() const = 0;
	virtual bool isPlaying() const = 0;

	virtual float getVolume() const = 0;
	virtual void setVolume(float volume) = 0;

	virtual ~BaseSound() = default;
};

}  // namespace Bloodlust
