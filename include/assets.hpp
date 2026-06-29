#pragma once

#include "datatype/soundClip.hpp"
#include "datatype/texture.hpp"

#include <SDL3/SDL_audio.h>
#include <memory>
#include <string>

namespace Bloodlust {

// Loads an asset from a path and returns both it and the size of it in bytes. Returns nullptr on
// failure.
std::unique_ptr<std::uint8_t[]> getAsset(const std::string& path, std::uint64_t& sizeOut);

// Loads a texture from a path and returns it, or nullptr on failure.
Texture* loadTexture(const std::string& path);

// Unloads a texture that was loaded from the given path from memory. If the texture was not already
// loaded, this function does nothing.
void unloadTexture(const std::string& path);

// Loads a sound clip from a path and returns it, or nullptr on failure.
std::unique_ptr<SoundClip> loadSoundClip(const char* path);

// Unloads all currently loaded assets from memory.
void unloadAssets();

}  // namespace Bloodlust
