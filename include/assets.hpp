#pragma once

#include "datatype/soundClip.hpp"
#include "datatype/texture.hpp"
#include "util.hpp"

#include <SDL3/SDL_audio.h>
#include <memory>
#include <string>

namespace Bloodlust {

// Loads an asset from a path and returns both it and the size of it in bytes. Returns nullptr on
// failure.
std::unique_ptr<uint8_t[]> getAsset(const std::string& path, uint64_t& sizeOut);

// Loads a texture from a path and returns it, or nullptr on failure.
[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
Texture* loadTexture(const char* path);

// Loads a sound clip from a path and returns it, or nullptr on failure.
[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
SoundClip* loadSoundClip(const char* path);

}  // namespace Bloodlust
