#pragma once

#include <SDL3/SDL_audio.h>
#include <cstdint>
#include <memory>
#include <string_view>

namespace Bloodlust {

class Texture;
class SoundData;

// Loads an asset from a path and returns both it and the size of it in bytes.
// Throws AssetLoadException if the asset could not be loaded or is empty.
std::unique_ptr<std::uint8_t[]> getAsset(std::string_view path, std::uint64_t& sizeOut);

// Loads a texture from a path and returns it.
// Throws AssetLoadException if the texture could not be loaded, or InternalError on failure.
Texture& loadTexture(std::string_view path);

// Unloads a texture that was loaded from the given path from memory. If the texture was not already
// loaded, this function does nothing.
void unloadTexture(std::string_view path) noexcept;

// Loads some sound data from a path and returns it.
// Throws AssetLoadException if the asset could not be loaded or is empty.
// Can also throw InternalError on failure.
SoundData& loadSoundData(std::string_view path);

// Unloads all currently loaded assets from memory.
void unloadAssets() noexcept;

}  // namespace Bloodlust
