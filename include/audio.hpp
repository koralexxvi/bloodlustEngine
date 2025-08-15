#pragma once

#include "util.hpp"

#include <cstdint>

namespace Bloodlust {

[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
bool initAudio();

bool isAudioDeviceOpen();

float getMasterVolume();
void setMasterVolume(float volume);

float getChannelVolume(uint32_t channel);
void setChannelVolume(uint32_t channel, float volume);

void stopAllAudio();

}  // namespace Bloodlust
