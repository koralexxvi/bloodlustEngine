#include "../../include/object/uiLabel.hpp"

#include "../../include/datatype/font.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::UILabel;

UILabel::UILabel(Font*) {}

UILabel* UILabel::copy() const {
	return new UILabel(nullptr);
}

bool UILabel::isTextVisible() const {
	return false;
}

bool UILabel::canDraw(const Vector2&, const Vector2&) const {
	return false;
}

bool UILabel::draw() {
	return true;
}

void UILabel::setFont(Font*) {}

void UILabel::setText(const std::string&) {}
