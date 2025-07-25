#include "../../include/object/uiLabel.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/font.hpp"
#include "../../include/logging.hpp"
#include "../../include/render/graphics.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::UILabel;

UILabel::UILabel(Font* initialFont) : font(initialFont) {
	textObject = TTF_CreateText(app->renderState.textEngine, font->getData(fontSize), text, 0);
	setTextColor(textColor);
}

UILabel* UILabel::copy() const {
	UILabel* copy = new UILabel(font);
	copy->setText(text);
	copy->setTextColor(textColor);
	copy->setFontAlignH(fontAlignH);
	copy->setFontAlignV(fontAlignV);
	copy->setFontSize(fontSize);
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

	SDL_FRect textRect = getTextBounds(drawRect);

#ifdef BL_DEBUG_DRAW_TEXT_BACKGROUND
	setDrawColor({0.5f, 0.0f, 0.5f, 0.25f});
	fillRect({textRect.x, textRect.y - textRect.h, textRect.w, textRect.h});
#endif

	setDrawColor(textColor);
	drawText(text, font, {textRect.x, textRect.y}, textRect.h, wrapText ? int(drawRect.w) : 0);
	return true;
}

void UILabel::setFont(Font* newFont) {
	if (newFont == nullptr) {
		setError("UILabel::setFont: passed font was null");
		return;
	}
	textFont = newFont->getData(fontSize);
}

const char* UILabel::getText() const {
	return text;
}

void UILabel::setText(const char* str) {
	text = str;
	TTF_SetTextString(textObject, str, 0);
}

float UILabel::getFontSize() const {
	return fontSize;
}

void UILabel::setFontSize(float newSize) {
	fontSize = newSize;
	TTF_SetFontSize(textFont, newSize);
}

Bloodlust::Color UILabel::getTextColor() const {
	return textColor;
}

void UILabel::setTextColor(const Color& color) {
	textColor = color;
	TTF_SetTextColorFloat(textObject, color.r, color.g, color.b, color.a);
}

Bloodlust::AlignModeHorizontal UILabel::getFontAlignH() const {
	return fontAlignH;
}

void UILabel::setFontAlignH(AlignModeHorizontal alignH) {
	fontAlignH = alignH;
	TTF_HorizontalAlignment alignment;
	switch (alignH) {
		case AlignModeHorizontal::LEFT: alignment = TTF_HORIZONTAL_ALIGN_LEFT; break;
		case AlignModeHorizontal::CENTER: alignment = TTF_HORIZONTAL_ALIGN_CENTER; break;
		case AlignModeHorizontal::RIGHT: alignment = TTF_HORIZONTAL_ALIGN_RIGHT; break;
		default: alignment = TTF_HORIZONTAL_ALIGN_INVALID; break;
	}
	TTF_SetFontWrapAlignment(textFont, alignment);
}

Bloodlust::AlignModeVertical UILabel::getFontAlignV() const {
	return fontAlignV;
}

void UILabel::setFontAlignV(AlignModeVertical alignV) {
	fontAlignV = alignV;
}

SDL_FRect UILabel::getTextBounds(const SDL_FRect& drawRect) const {
	Vector2 stringSize = font->getStringSize(text, fontSize, wrapText ? int(drawRect.w) : 0);
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
