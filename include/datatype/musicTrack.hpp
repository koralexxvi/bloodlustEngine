#ifndef BL_DATA_MUSICTRACK_H
#define BL_DATA_MUSICTRACK_H

#include "baseSound.hpp"

#include <SDL3_mixer/SDL_mixer.h>

namespace Bloodlust {

class MusicTrack : public BaseSound {
  private:
	Mix_Music* musicData;

  public:
	MusicTrack(Mix_Music* data, int loopCount = 0);

	void play() override;
	void pause() const override;
	void stop() const override;
	bool isPlaying() const override;

	float getVolume() const override;
	void setVolume(float volume) override;

	void seekTo(double time) const;
	double getTimePosition() const;

	~MusicTrack();
};

}  // namespace Bloodlust

#endif
