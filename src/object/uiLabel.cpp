#include "../../include/object/uiLabel.hpp"

#include "../../include/datatype/font.hpp"
#include "../../include/graphics.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

using Bloodlust::UILabel;

UILabel::UILabel(Font* initialFont) : font(initialFont) {}

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

	Vector2 drawSize = {drawRect.w, drawRect.h};
	if (drawSize != cachedSize) {
		isDirty |= 0b10;
	}
	if (isDirty != 0) {
		if (isDirty & 0b10) {
			cachedSize = drawSize;
		}
		if (!update()) {
			return false;
		}
	}

	SDL_FRect textRect = getTextBounds(drawRect);

#ifdef BL_DEBUG_DRAW_TEXT_BACKGROUND
	setDrawColor({0.5f, 0.0f, 0.5f, 0.25f});
	fillRect(textRect);
#endif

	setDrawColor(textColor);
	drawText(text.c_str(), font, {textRect.x, textRect.y}, fontSize);
	return true;
}

bool UILabel::isValid() const {
	return font != nullptr;
}

void UILabel::markDirty() {
	isDirty = 0b11;
}

void UILabel::setFont(Font* newFont) {
	font = newFont;
}

void UILabel::setFontSize(float newSize) {
	fontSize = newSize;
	isDirty |= 0b10;
}

void UILabel::setFontAlignHorizontal(AlignModeHorizontal align) {
	alignH = align;
	isDirty |= 0b01;
}

void UILabel::setFontAlignVertical(AlignModeVertical align) {
	alignV = align;
}

void UILabel::setText(const std::string& str) {
	text = str;
}

void UILabel::setTextColor(const Color& color) {
	textColor = color;
}

bool UILabel::update() {
	TTF_Font* fontData = font->getData(fontSize);
	if (isDirty & 0b01) {
		TTF_HorizontalAlignment alignment;
		switch (alignH) {
			case AlignModeHorizontal::LEFT: alignment = TTF_HORIZONTAL_ALIGN_LEFT; break;
			case AlignModeHorizontal::CENTER: alignment = TTF_HORIZONTAL_ALIGN_CENTER; break;
			case AlignModeHorizontal::RIGHT: alignment = TTF_HORIZONTAL_ALIGN_RIGHT; break;
			default: return false;
		}
		TTF_SetFontWrapAlignment(fontData, alignment);
		isDirty ^= 0b01;
	}
	if (isDirty & 0b10) {
		if (fontSize > 0.0f) {
			isDirty ^= TTF_SetFontSize(fontData, fontSize) << 1;
		} else {
			isDirty ^= TTF_SetFontSize(fontData, cachedSize.y) << 1;
		}
	}
	return isDirty == 0;
}

SDL_FRect UILabel::getTextBounds(const SDL_FRect& drawRect) const {
	SDL_FRect textRect = {0.0f, 0.0f, 0.0f, fontSize};
	switch (alignH) {
		case AlignModeHorizontal::LEFT: textRect.x = drawRect.x; break;
		case AlignModeHorizontal::RIGHT: textRect.x = drawRect.x + drawRect.w - fontSize; break;
		default: textRect.x = drawRect.x + (drawRect.w - fontSize) / 2.0f; break;
	}
	switch (alignV) {
		case AlignModeVertical::TOP: textRect.y = drawRect.y - fontSize; break;
		case AlignModeVertical::CENTER: textRect.y = drawRect.y - fontSize; break;
		default: textRect.y = drawRect.y - fontSize; break;
	}
	return textRect;
}
