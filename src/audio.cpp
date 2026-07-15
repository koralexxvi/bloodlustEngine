#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_init.h>
#include <bloodlustEngine/app.hpp>
#include <bloodlustEngine/audio.hpp>
#include <bloodlustEngine/datatype/soundChannel.hpp>
#include <bloodlustEngine/exception/internalError.hpp>
#include <bloodlustEngine/logging.hpp>

void Bloodlust::initAudio() {
	if (!SDL_Init(SDL_INIT_AUDIO)) {
		throw InternalError("Could not init audio: " + getError());
	}
	app->audioState.deviceOut = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	if (!isAudioDeviceOpen()) {
		// TODO: should this really be throwing an error?
		throw InternalError("Failed to open audio device: " + getError());
	}
	for (std::size_t i = 0; i < BL_SOUND_CHANNEL_COUNT; i++) {
		app->audioState.channels[i] = std::make_unique<SoundChannel>();
	}
}

bool Bloodlust::isAudioDeviceOpen() noexcept {
	return app != nullptr && app->audioState.deviceOut != 0;
}

void Bloodlust::processAudio() {
	if (!isAudioDeviceOpen()) {
		return;
	}
	for (const auto& v : app->audioState.channels) {
		v->update();
	}
}

float Bloodlust::getMasterVolume() noexcept {
	if (!isAudioDeviceOpen()) {
		return 0.0f;
	}
	return SDL_GetAudioDeviceGain(app->audioState.deviceOut);
}

void Bloodlust::setMasterVolume(float volume) noexcept {
	if (isAudioDeviceOpen()) {
		SDL_SetAudioDeviceGain(app->audioState.deviceOut, volume);
	}
}

float Bloodlust::getChannelVolume(std::uint32_t channel) noexcept {
	if (!isAudioDeviceOpen()) {
		return 0.0f;
	}
	SDL_assert(channel < BL_SOUND_CHANNEL_COUNT);
	return app->audioState.channels[channel]->getVolume();
}

void Bloodlust::setChannelVolume(std::uint32_t channel, float volume) noexcept {
	if (isAudioDeviceOpen()) {
		SDL_assert(channel < BL_SOUND_CHANNEL_COUNT);
		SDL_assert(volume >= 0.0f);
		app->audioState.channels[channel]->setVolume(volume);
	}
}

void Bloodlust::stopAllAudio() noexcept {
	if (!isAudioDeviceOpen()) {
		return;
	}
	for (const auto& v : app->audioState.channels) {
		v->stopChannel();
	}
}

void Bloodlust::stopChannel(std::uint32_t channel) noexcept {
	if (isAudioDeviceOpen()) {
		app->audioState.channels[channel]->stopChannel();
	}
}

void Bloodlust::closeAudio() noexcept {
	if (app == nullptr) {
		return;
	}
	app->audioState.channels = {};
	if (isAudioDeviceOpen()) {
		SDL_CloseAudioDevice(app->audioState.deviceOut);
		app->audioState.deviceOut = 0;
	}
}
