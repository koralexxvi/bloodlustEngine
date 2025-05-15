#ifndef BL_DATA_TEXTURE_H
#define BL_DATA_TEXTURE_H

#include "color.hpp"
#include "vector2.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

namespace Bloodlust::DataTypes {

enum class TextureFilterType {
	NEAREST,
	LINEAR
};

class Texture {
  private:
	static SDL_ScaleMode defaultScaleMode;
	SDL_Texture* textureData = nullptr;

  public:
	static void setDefaultFilterType(TextureFilterType type);

	Texture(SDL_Texture* data);

	SDL_Texture* getData() const;
	int getHeight() const;
	int getWidth() const;
	Vector2 getSize() const;

	void setColor(const Color& color);

	~Texture();
};

}  // namespace Bloodlust::DataTypes

#endif
