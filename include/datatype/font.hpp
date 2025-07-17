#pragma once

#include "vector2.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#define BL_MAX_FONT_SIZE (96.0f)

namespace Bloodlust {

class Font {
  private:
	TTF_Font* fontData[20] = {};

	static int getNearestFontSize(float size);
	static int getFontSizeIndex(float size);

  public:
	Font(const char* fontPath, bool& success);
	Font(TTF_Font* font);

	TTF_Font* getData(float height);
	Vector2 getStringSize(const char* str, float height);

	~Font();
};

}  // namespace Bloodlust
