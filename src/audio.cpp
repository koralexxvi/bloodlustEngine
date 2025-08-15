#include "../include/audio.hpp"

#include "../include/app.hpp"

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
	SDL_AudioSpec spec;
	if (!SDL_GetAudioDeviceFormat(app->audioState.deviceOut, &spec, nullptr)) {
		return false;
	}
	for (size_t i = 0; i < BL_AUDIO_STREAM_COUNT; i++) {
		SDL_AudioStream* stream = SDL_CreateAudioStream(&spec, &spec);
		if (stream == nullptr) {
			return false;
		}
		app->audioState.audioStreams[i] = stream;
	}
	return SDL_BindAudioStreams(
		app->audioState.deviceOut,
		app->audioState.audioStreams,
		BL_AUDIO_STREAM_COUNT
	);
}

bool Bloodlust::isAudioDeviceOpen() {
	return app->audioState.deviceOut != 0;
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

float Bloodlust::getChannelVolume(uint32_t channel) {
	if (!isAudioDeviceOpen()) {
		return 0.0f;
	}
	return SDL_GetAudioStreamGain(app->audioState.audioStreams[channel]);
}

void Bloodlust::setChannelVolume(uint32_t channel, float volume) {
	if (isAudioDeviceOpen()) {
		SDL_SetAudioStreamGain(app->audioState.audioStreams[channel], volume);
	}
}

void Bloodlust::stopAllAudio() {}

void Bloodlust::closeAudio() {
	if (app == nullptr) {
		return;
	}
	for (size_t i = 0; i < BL_AUDIO_STREAM_COUNT; i++) {
		SDL_DestroyAudioStream(app->audioState.audioStreams[i]);
	}
	if (isAudioDeviceOpen()) {
		SDL_CloseAudioDevice(app->audioState.deviceOut);
	}
}
