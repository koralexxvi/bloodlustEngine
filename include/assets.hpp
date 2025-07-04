#ifndef BL_ASSETS_H
#define BL_ASSETS_H

#include "datatype/musicTrack.hpp"
#include "datatype/soundClip.hpp"
#include "datatype/texture.hpp"

#include <string>

namespace Bloodlust {

SDL_IOStream* getAsset(const std::string& path, void** dataOut, uint64_t* sizeOut);
Texture* loadTexture(const std::string& path);
SoundClip* loadSoundClip(const std::string& path);
MusicTrack* loadMusicTrack(const std::string& path);

}  // namespace Bloodlust

#endif
