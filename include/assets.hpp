#ifndef BL_ASSETS_H
#define BL_ASSETS_H

#include "datatype/musicTrack.hpp"
#include "datatype/soundClip.hpp"
#include "datatype/texture.hpp"

#include <SDL3/SDL_render.h>
#include <string>

namespace Bloodlust::Assets {
using namespace DataTypes;

SDL_IOStream* getAsset(const char* path, void** dataOut, uint64_t* sizeOut);
Texture* loadTexture(SDL_Renderer* renderer, std::string path);
SoundClip* loadSoundClip(std::string path);
MusicTrack* loadMusicTrack(std::string path);

}  // namespace Bloodlust::Assets

#endif
