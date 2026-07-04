#pragma once

#include "util.hpp"

#include <cstdint>

namespace Bloodlust {

// Initialises the audio subsystem and returns true on success.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool initAudio() noexcept;

// Returns true if the audio device is open.
bool isAudioDeviceOpen() noexcept;

// Updates the audio mixer, playing audio and queuing samples when needed.
[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool processAudio() noexcept;

// Returns the master volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
float getMasterVolume() noexcept;

// Sets the master volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
void setMasterVolume(float volume) noexcept;

// Returns the given channel's volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
// If the channel is greater than or equal to BL_SOUND_CHANNEL_COUNT, the behaviour is undefined.
float getChannelVolume(std::uint32_t channel) noexcept;

// Sets the given channel's volume. 1.0f is full volume, while 0.0f is silence. Can exceed 1.0f.
// If the channel is greater than or equal to BL_SOUND_CHANNEL_COUNT, the behaviour is undefined.
void setChannelVolume(std::uint32_t channel, float volume) noexcept;

// Stops all audio on every channel.
void stopAllAudio() noexcept;

// Stops all audio on the given channel. If the channel is greater than or equal to
// BL_SOUND_CHANNEL_COUNT, the behaviour is undefined.
void stopChannel(std::uint32_t channel) noexcept;

// Closes the audio subsystem.
void closeAudio() noexcept;

}  // namespace Bloodlust
