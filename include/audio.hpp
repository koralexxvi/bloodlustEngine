#pragma once

#include "util.hpp"

#include <cstdint>

namespace Bloodlust {

// Initialises the audio subsystem and returns true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool initAudio();

// Returns true if the audio device is open.
bool isAudioDeviceOpen();

// Updates the audio mixer, playing audio and queuing samples when needed.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool processAudio();

// Returns the master volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
float getMasterVolume();

// Sets the master volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
void setMasterVolume(float volume);

// Returns the given channel's volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
// If the channel is greater than or equal to BL_SOUND_CHANNEL_COUNT, the behaviour is undefined.
float getChannelVolume(std::uint32_t channel);

// Sets the given channel's volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
// If the channel is greater than or equal to BL_SOUND_CHANNEL_COUNT, the behaviour is undefined.
void setChannelVolume(std::uint32_t channel, float volume);

// Stops all audio on every channel.
void stopAllAudio();

// Stops all audio on the given channel. If the channel is greater than or equal to
// BL_SOUND_CHANNEL_COUNT, the behaviour is undefined.
void stopChannel(std::uint32_t channel);

// Closes the audio subsystem.
void closeAudio();

}  // namespace Bloodlust
