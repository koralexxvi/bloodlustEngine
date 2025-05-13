#ifndef BL_DATA_FONT_H
#define BL_DATA_FONT_H

#include <SDL3_ttf/SDL_ttf.h>

namespace Bloodlust::DataTypes {

class Font {
  private:
	TTF_Font* fontData;

  public:
	Font(const char* fontPath, float pointSize = 14.0f);

	bool isValid();

	~Font();
};

}  // namespace Bloodlust::DataTypes

#endif
