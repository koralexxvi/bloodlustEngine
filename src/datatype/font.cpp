#include "../../include/datatype/font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::Font;

Bloodlust::Vector2 Font::getStringSize(const char*, TTF_Font*, float, float) {
	return {-1.0f, -1.0f};
}

Font::Font(const char*, bool& success) {
	success = true;
}

Font::~Font() {}
