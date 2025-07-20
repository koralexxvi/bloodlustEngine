#pragma once

#include "datatype/musicTrack.hpp"
#include "datatype/soundClip.hpp"
#include "datatype/texture.hpp"

#include <memory>
#include <string>

namespace Bloodlust {

std::unique_ptr<uint8_t[]> getAsset(const std::string& path, uint64_t& sizeOut);
Texture* loadTexture(const char* path);
SoundClip* loadSoundClip(const char* path);
MusicTrack* loadMusicTrack(const char* path);

}  // namespace Bloodlust
