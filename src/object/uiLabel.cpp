#include "../../include/object/uiLabel.hpp"

#include "../../include/datatype/font.hpp"
#include "../../include/graphics.hpp"

#include <string>

using Bloodlust::Objects::UILabel;

bool UILabel::canDraw() const {
	return visible && (textColor.a != 0 || backgroundColor.a != 0);
}

bool UILabel::draw() {
	Vector2 drawPos = getDrawPos();
	Vector2 drawSize = getDrawSize();

	if (backgroundColor.a != 0 && !Graphics::drawRect(drawPos, drawSize)) {
		return false;
	}

	return false;
}

void UILabel::setFont(DataTypes::Font* newFont) {
	font = newFont;
}

void UILabel::setText(const std::string& str) {}
