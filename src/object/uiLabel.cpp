#include "../../include/object/uiLabel.hpp"

#include "../../include/datatype/font.hpp"
#include "../../include/logging.hpp"
#include "../../include/render/graphics.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::UILabel;

UILabel::UILabel(Font* initialFont) : font(initialFont) {}

UILabel* UILabel::copy() const {
	UILabel* copy = new UILabel(font);
	copy->text = text;
	copy->textColor = textColor;
	copy->fontAlignH = fontAlignH;
	copy->fontAlignV = fontAlignV;
	copy->setFontSize(fontSize);
	return copy;
}

bool UILabel::canDraw() const {
	return visible
		&& (textColor.a > 0.0f || backgroundColor.a > 0.0f
			|| (outlineWidth > 0 && outlineColor.a > 0.0f));
}

bool UILabel::draw() {
	SDL_FRect drawRect = getDrawRect();
	if (drawRect.w == 0.0f || drawRect.h == 0.0f) {
		return true;
	}

	if (backgroundColor.a > 0.0f) {
		setDrawColor(backgroundColor);
		fillRect(drawRect);
	}

	if (outlineWidth > 0 && outlineColor.a > 0.0f) {
		if (!drawOutline()) {
			return false;
		}
	}

	if (textColor.a <= 0.0f) {
		return true;
	}

	SDL_FRect textRect = getTextBounds(drawRect);

#ifdef BL_DEBUG_DRAW_TEXT_BACKGROUND
	setDrawColor({0.5f, 0.0f, 0.5f, 0.25f});
	fillRect(textRect);
#endif

	setDrawColor(textColor);
	drawText(text.c_str(), font, {textRect.x, textRect.y}, textRect.h);
	return true;
}

void UILabel::setFont(Font* newFont) {
	if (newFont == nullptr) {
		setError("UILabel::setFont: passed font was null");
		return;
	}
	font = newFont;
}

void UILabel::setFontSize(float newSize) {
	fontSize = newSize;
}

SDL_FRect UILabel::getTextBounds(const SDL_FRect& drawRect) const {
	float fontDrawSize = fontSize > 0.0f ? fontSize : drawRect.h;
	Vector2 stringSize = font->getStringSize(text.c_str(), fontDrawSize);
	SDL_FRect textRect = {0.0f, 0.0f, stringSize.x, fontDrawSize};
	switch (fontAlignH) {
		case AlignModeHorizontal::LEFT: textRect.x = drawRect.x; break;
		case AlignModeHorizontal::CENTER:
			textRect.x = drawRect.x + (drawRect.w - textRect.w) * 0.5f;
			break;
		case AlignModeHorizontal::RIGHT: textRect.x = drawRect.x + drawRect.w - textRect.w; break;
		default: break;
	}
	switch (fontAlignV) {
		case AlignModeVertical::TOP: textRect.y = drawRect.y + stringSize.y; break;
		case AlignModeVertical::CENTER:
			textRect.y = (drawRect.y + stringSize.y) + (drawRect.h - stringSize.y) * 0.5f;
			break;
		case AlignModeVertical::BOTTOM: textRect.y = drawRect.y + drawRect.h; break;
		default: break;
	}

	return textRect;
}
