#pragma once

#include <SDL3_ttf/SDL_ttf.h>

namespace Bloodlust {

class Font {
  private:
	TTF_Font* fontData;
	float pointSize;

	[[maybe_unused]]
	char _padding[4];

  public:
	Font(const char* fontPath, float pointSize = 14.0f);
	Font(TTF_Font* fontData);

	bool isValid() const;
	TTF_Font* getData() const;
	float getPointSize() const;

	bool rescaleTo(float newSize);

	~Font();
};

}  // namespace Bloodlust
