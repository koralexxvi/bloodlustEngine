#include "../include/audio.hpp"

#include "../include/app.hpp"
#include "../include/datatype/soundChannel.hpp"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_init.h>

bool Bloodlust::initAudio() {
	if (!SDL_Init(SDL_INIT_AUDIO)) {
		return false;
	}
	app->audioState.deviceOut = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	if (!isAudioDeviceOpen()) {
		return false;
	}
	bool success;
	for (std::size_t i = 0; i < BL_SOUND_CHANNEL_COUNT; i++) {
		app->audioState.channels[i] = std::make_unique<SoundChannel>(success);
		if (!success) {
			app->audioState.channels = {};
			return false;
		}
	}
	return true;
}

bool Bloodlust::isAudioDeviceOpen() {
	return app->audioState.deviceOut != 0;
}

bool Bloodlust::processAudio() {
	if (isAudioDeviceOpen()) {
		for (const auto& v : app->audioState.channels) {
			if (!v->update()) {
				return false;
			}
		}
	}
	return true;
}

float Bloodlust::getMasterVolume() {
	if (!isAudioDeviceOpen()) {
		return 0.0f;
	}
	return SDL_GetAudioDeviceGain(app->audioState.deviceOut);
}

void Bloodlust::setMasterVolume(float volume) {
	if (isAudioDeviceOpen()) {
		SDL_SetAudioDeviceGain(app->audioState.deviceOut, volume);
	}
}

float Bloodlust::getChannelVolume(std::uint32_t channel) {
	if (!isAudioDeviceOpen()) {
		return 0.0f;
	}
	SDL_assert(channel < BL_SOUND_CHANNEL_COUNT);
	return app->audioState.channels[channel]->getVolume();
}

void Bloodlust::setChannelVolume(std::uint32_t channel, float volume) {
	if (isAudioDeviceOpen()) {
		SDL_assert(channel < BL_SOUND_CHANNEL_COUNT);
		SDL_assert(volume >= 0.0f);
		app->audioState.channels[channel]->setVolume(volume);
	}
}

void Bloodlust::stopAllAudio() {
	if (!isAudioDeviceOpen()) {
		return;
	}
	for (const auto& v : app->audioState.channels) {
		v->stopChannel();
	}
}

void Bloodlust::stopChannel(std::uint32_t channel) {
	if (isAudioDeviceOpen()) {
		app->audioState.channels[channel]->stopChannel();
	}
}

void Bloodlust::closeAudio() {
	if (app == nullptr) {
		return;
	}
	app->audioState.channels = {};
	if (isAudioDeviceOpen()) {
		SDL_CloseAudioDevice(app->audioState.deviceOut);
	}
}
