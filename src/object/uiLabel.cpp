#include "../../include/object/uiLabel.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/font.hpp"
#include "../../include/logging.hpp"
#include "../../include/render/graphics.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::UILabel;

UILabel::UILabel(Font* initialFont) : font(initialFont) {
	textFont = font->getData(style, Font::alignHToTTFAlign(fontAlignH));
	textObject = TTF_CreateText(app->renderState.textEngine, textFont, text.c_str(), 0);
	setTextColor(textColor);
	setFontSize(fontSize);
}

UILabel* UILabel::copy() const {
	UILabel* copy = new UILabel(font);
	copy->setText(text);
	copy->setFontAlignH(fontAlignH);
	copy->setFontAlignV(fontAlignV);
	copy->setTextColor(textColor);
	copy->setFontSize(fontSize);
	copy->setFontStyle(style);
	return copy;
}

bool UILabel::isTextVisible() const {
	return textColor.a > 0.0f && (scaleText || fontSize > 0.0f);
}

bool UILabel::canDraw() const {
	return visible && (backgroundColor.a > 0.0f || isTextVisible() || isOutlineVisible());
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
	if (isOutlineVisible()) {
		setDrawColor(outlineColor);
		drawRectOutline(drawRect, outlineWidth);
	}

	if (textColor.a <= 0.0f) {
		return true;
	}

	if (fontDirty) {
		textFont = font->getData(style, Font::alignHToTTFAlign(fontAlignH));
		fontDirty = false;
	}

	SDL_FRect textRect = getTextBounds(drawRect);
#ifdef BL_DEBUG_DRAW_TEXT_BACKGROUND
	setDrawColor({0.5f, 0.0f, 0.5f, 0.25f});
	fillRect({textRect.x, textRect.y - textRect.h, textRect.w, textRect.h});
#endif

	setDrawColor(textColor);
	drawText(textObject, {textRect.x, textRect.y}, textRect.h);
	return true;
}

void UILabel::setFont(Font* newFont) {
	if (newFont == nullptr) {
		setError("UILabel::setFont: passed font was null");
		return;
	}
	font = newFont;
	fontDirty = true;
}

std::string UILabel::getText() const {
	return text;
}

void UILabel::setText(const std::string& str) {
	text = str;
	TTF_SetTextString(textObject, str.c_str(), 0);
}

float UILabel::getFontSize() const {
	return fontSize;
}

void UILabel::setFontSize(float newSize) {
	fontSize = newSize;
}

Bloodlust::Color UILabel::getTextColor() const {
	return textColor;
}

void UILabel::setTextColor(const Color& color) {
	textColor = color;
}

TTF_FontStyleFlags UILabel::getFontStyle() const {
	return style;
}

void UILabel::setFontStyle(TTF_FontStyleFlags newStyle) {
	style = newStyle;
	fontDirty = true;
}

Bloodlust::AlignModeHorizontal UILabel::getFontAlignH() const {
	return fontAlignH;
}

void UILabel::setFontAlignH(AlignModeHorizontal alignH) {
	fontAlignH = alignH;
	fontDirty = true;
}

Bloodlust::AlignModeVertical UILabel::getFontAlignV() const {
	return fontAlignV;
}

void UILabel::setFontAlignV(AlignModeVertical alignV) {
	fontAlignV = alignV;
}

SDL_FRect UILabel::getTextBounds(const SDL_FRect& drawRect) const {
	Vector2 stringSize =
		font->getStringSize(text.c_str(), textFont, fontSize, wrapText ? int(drawRect.w) : 0);
	if (scaleText) {
		float scaleX = drawRect.w / stringSize.x;
		float scaleY = drawRect.h / stringSize.y;
		stringSize *= scaleX < scaleY ? scaleX : scaleY;
	}
	SDL_FRect textRect = {0.0f, 0.0f, stringSize.x, stringSize.y};
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
