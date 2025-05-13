#include "../../include/datatype/musicTrack.hpp"

#include <SDL3_mixer/SDL_mixer.h>

using Bloodlust::DataTypes::MusicTrack;

MusicTrack::MusicTrack(Mix_Music* musicData, int loopCount) : musicData(musicData) {
	this->loopCount = loopCount;
}

void MusicTrack::play() {
	Mix_HaltMusic();
	Mix_PlayMusic(musicData, loopCount);
}

void MusicTrack::pause() const {
	Mix_PauseMusic();
}

void MusicTrack::stop() const {
	Mix_PauseMusic();
	Mix_RewindMusic();
}

bool MusicTrack::isPlaying() const {
	return Mix_PlayingMusic() && !Mix_PausedMusic();
}

float MusicTrack::getVolume() const {
	return float(Mix_VolumeMusic(-1)) / MIX_MAX_VOLUME;
}

void MusicTrack::setVolume(float volume) {
	Mix_VolumeMusic(int(MIX_MAX_VOLUME * volume));
}

void MusicTrack::seekTo(double time) const {
	Mix_SetMusicPosition(time);
}

double MusicTrack::getTimePosition() const {
	return Mix_GetMusicPosition(musicData);
}

MusicTrack::~MusicTrack() {
	Mix_HaltMusic();
	Mix_FreeMusic(musicData);
	musicData = nullptr;
}
