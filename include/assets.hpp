#pragma once

#include "datatype/musicTrack.hpp"
#include "datatype/soundClip.hpp"
#include "datatype/texture.hpp"
#include "util.hpp"

#include <memory>
#include <string>

namespace Bloodlust {

std::unique_ptr<uint8_t[]> getAsset(const std::string& path, uint64_t& sizeOut);

[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
Texture* loadTexture(const char* path);

[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
SoundClip* loadSoundClip(const char* path);

[[nodiscard(BL_NODISCARD_LEAK_MSG)]]
MusicTrack* loadMusicTrack(const char* path);

}  // namespace Bloodlust
