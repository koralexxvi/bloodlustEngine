#pragma once

#include "util.hpp"

#include <cstdint>

namespace Bloodlust {

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool initAudio();

bool isAudioDeviceOpen();

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool processAudio();

float getMasterVolume();
void setMasterVolume(float volume);

float getChannelVolume(uint32_t channel);
void setChannelVolume(uint32_t channel, float volume);

void stopAllAudio();
void stopChannel(uint32_t channel);

void closeAudio();

}  // namespace Bloodlust
