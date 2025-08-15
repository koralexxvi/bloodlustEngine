#pragma once

#include "datatype/soundClip.hpp"
#include "datatype/texture.hpp"
#include "util.hpp"

#include <SDL3/SDL_audio.h>
#include <memory>
#include <string>

namespace Bloodlust {

std::unique_ptr<uint8_t[]> getAsset(const std::string& path, uint64_t& sizeOut);

[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
Texture* loadTexture(const char* path);

[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
SoundClip* loadSoundClip(
	const char* path,
	const SDL_AudioSpec& format = {.format = SDL_AUDIO_F32, .channels = 2, .freq = 48'000}
);

}  // namespace Bloodlust
