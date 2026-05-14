#pragma once

#ifndef BL_FONT_SIZE
	#define BL_FONT_SIZE (64.0f)
#endif

namespace Bloodlust {

// A class representing a font face.
class Font {
  public:
	// Constructs a new font from the given path, and fills success with true if successful.
	Font(const char* fontPath, bool& success);

	// Destroys this font, freeing each TTF_Font associated with it. All pointers to TTF_Font
	// structs associated with this will be closed and now be invalid.
	~Font();
};

}  // namespace Bloodlust
