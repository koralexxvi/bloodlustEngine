#include "../../include/datatype/color.hpp"

using Bloodlust::DataTypes::Color;

void Color::toFloat(float* fr, float* fg, float* fb, float* fa) const {
	if (fr != nullptr) {
		*fr = r / 255.0f;
	}
	if (fg != nullptr) {
		*fg = g / 255.0f;
	}
	if (fb != nullptr) {
		*fb = b / 255.0f;
	}
	if (fa != nullptr) {
		*fa = a / 255.0f;
	}
}
