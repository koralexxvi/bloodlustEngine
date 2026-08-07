#include <SDL3/SDL_stdinc.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/asset/soundClip.hpp>
#include <bloodlustEngine/assets.hpp>
#include <bloodlustEngine/audio.hpp>
#include <bloodlustEngine/exception/assetLoadException.hpp>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>

using Bloodlust::SoundClip;

SoundClip::~SoundClip() noexcept {
	stop();
}

void SoundClip::play() {
	if (isAudioDeviceOpen()) {
		app->audioState.channels[channel]->playSound(this);
	}
}

void SoundClip::pause() const noexcept {}

void SoundClip::stop() const noexcept {
	if (isAudioDeviceOpen() && isPlaying()) {
		stopChannel(channel);
	}
}

bool SoundClip::isPlaying() const noexcept {
	return isAudioDeviceOpen() && app->audioState.channels[channel]->getPlayingSound() == this;
}

void SoundClip::setVolume(float newVolume) noexcept {
	volume = newVolume;
	if (isPlaying()) {
		app->audioState.channels[channel]->updateVolume();
	}
}
