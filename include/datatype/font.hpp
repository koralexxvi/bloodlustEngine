#pragma once

#include "vector2.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#ifndef BL_FONT_SIZE
	#define BL_FONT_SIZE (64.0f)
#endif

namespace Bloodlust {

// A class representing a font face.
class Font {
  public:
	// Returns the size that a string would take up given a string, font and height.
	// If maxLen is greater than 0.0f, then the string will wrap at that point. Newlines are always
	// wrapped.
	static Vector2 getStringSize(
		const char* str,
		TTF_Font* font,
		float height,
		float maxLen = 0.0f
	);

	// Constructs a new font from the given path, and fills success with true if successful.
	Font(const char* fontPath, bool& success);

	// Destroys this font, freeing each TTF_Font associated with it. All pointers to TTF_Font
	// structs associated with this will be closed and now be invalid.
	~Font();
};

}  // namespace Bloodlust
