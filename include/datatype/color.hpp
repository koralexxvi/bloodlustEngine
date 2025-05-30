#ifndef BL_DATA_COLOR_H
#define BL_DATA_COLOR_H

#include <SDL3/SDL_pixels.h>
#include <cstdint>

namespace Bloodlust {

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a = 255;

	void toFloat(float* fr, float* fg, float* fb, float* fa) const;
	SDL_Color toSDLColor() const;

	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;
};

}  // namespace Bloodlust

#endif
