#pragma once

#include "../include/datatype/soundClip.hpp"

#include <SDL3/SDL_audio.h>
#include <cstdint>
#include <memory>
#include <string_view>

namespace Bloodlust {

class Texture;

// Loads an asset from a path and returns both it and the size of it in bytes. Returns nullptr on
// failure.
std::unique_ptr<std::uint8_t[]> getAsset(std::string_view path, std::uint64_t& sizeOut) noexcept;

// Loads a texture from a path and returns it, or nullptr on failure.
Texture* loadTexture(std::string_view path) noexcept;

// Unloads a texture that was loaded from the given path from memory. If the texture was not already
// loaded, this function does nothing.
void unloadTexture(std::string_view path) noexcept;

// Loads a sound clip from a path and returns it, or nullptr on failure.
std::unique_ptr<SoundClip> loadSoundClip(std::string_view path) noexcept;

// Unloads all currently loaded assets from memory.
void unloadAssets() noexcept;

}  // namespace Bloodlust
