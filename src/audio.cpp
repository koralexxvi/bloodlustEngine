#include "../include/audio.hpp"

#include <SDL3_mixer/SDL_mixer.h>

using namespace Bloodlust;

float Audio::getMasterVolume() {
	return float(Mix_MasterVolume(-1)) / MIX_MAX_VOLUME;
}

void Audio::setMasterVolume(float volume) {
	Mix_MasterVolume(int(MIX_MAX_VOLUME / volume));
}

void Audio::stopAllAudio() {
	Mix_HaltMusic();
	Mix_HaltChannel(-1);
}
