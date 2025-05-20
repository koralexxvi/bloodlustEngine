#include "../../include/object/uiLabel.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/font.hpp"
#include "../../include/graphics.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <string>

using Bloodlust::UILabel;

UILabel::UILabel(Font* initialFont) {
	TTF_Font* copy = TTF_CopyFont(initialFont->getData());
	if (copy != nullptr) {
		setFont(new Font(copy));
	} else {
		font = nullptr;
	}
}

bool UILabel::canDraw() const {
	return visible && ((textColor.a != 0 && textScale > 0.0f) || backgroundColor.a != 0);
}

bool UILabel::draw() {
	SDL_FRect drawRect = getDrawRect();

	if (backgroundColor.a != 0) {
		Graphics::setDrawColor(backgroundColor);
		if (!Graphics::fillRect(&drawRect)) {
			return false;
		}
	}

	if (textColor.a == 0 || textScale <= 0.0f || drawRect.h == 0.0f) {
		return true;
	}

	Vector2 drawSize = {drawRect.w, drawRect.h};
	if (drawSize != cachedSize) {
		isDirty |= 0b0010;
	}
	if (isDirty != 0) {
		if (isDirty & 0b0010) {
			cachedSize = drawSize;
		}
		if (!update()) {
			return false;
		}
	}

	SDL_FRect textRect = getTextBounds(drawRect);

#ifdef BL_DEBUG_DRAW_TEXT_BACKGROUND
	Graphics::setDrawColor({128, 0, 128, 64});
	Graphics::fillRect(&textRect);
#endif

	return TTF_DrawRendererText(textObject, textRect.x, textRect.y);
}

bool UILabel::isValid() const {
	return font != nullptr;
}

void UILabel::markDirty() {
	isDirty = 0b1111;
}

void UILabel::setFont(Font* newFont) {
	font = std::unique_ptr<Font>(newFont);
	isDirty |= 0b1000;
}

void UILabel::setText(const std::string& str) {
	text = str;
	isDirty |= 0b0100;
}

void UILabel::setTextScale(float newScale) {
	textScale = newScale;
	isDirty |= 0b0010;
}

void UILabel::setTextColor(const Color& color) {
	textColor = color;
	isDirty |= 0b0001;
}

bool UILabel::update() {
	if (textObject == nullptr) {
		TTF_Text* newText = TTF_CreateText(app->textEngine, font->getData(), text.c_str(), 0);
		if (newText == nullptr) {
			return false;
		}
		TTF_DestroyText(textObject);
		textObject = newText;
		isDirty &= 0b0011;
	}

	if (isDirty & 0b1000) {
		isDirty ^= TTF_SetTextFont(textObject, font->getData()) << 3;
	}
	if (isDirty & 0b0100) {
		isDirty ^= TTF_SetTextString(textObject, text.c_str(), 0) << 2;
	}
	if (isDirty & 0b0010) {
		isDirty ^= TTF_SetFontSize(font->getData(), cachedSize.y * textScale) << 1;
	}
	if (isDirty & 0b0001) {
		isDirty ^= TTF_SetTextColor(textObject, textColor.r, textColor.g, textColor.b, textColor.a);
	}

	int w, h;
	if (!TTF_GetTextSize(textObject, &w, &h)) {
		return false;
	}
	textSize = {float(w) * textScale, float(h) * textScale};

	return isDirty == 0;
}

SDL_FRect UILabel::getTextBounds(const SDL_FRect& drawRect) const {
	SDL_FRect textRect = {};
	switch (textAlignH) {
		using enum AlignModeHorizontal;
		case LEFT: textRect.x = drawRect.x; break;
		case RIGHT: textRect.x = (drawRect.x + drawRect.w) - textSize.x; break;
		default: textRect.x = drawRect.x + (drawRect.w - textSize.x) / 2.0f; break;
	}
	switch (textAlignV) {
		using enum AlignModeVertical;
		case TOP: textRect.y = drawRect.y; break;
		case BOTTOM: textRect.y = (drawRect.y + drawRect.h) - textSize.y; break;
		default: textRect.y = drawRect.y + (drawRect.h - textSize.y) / 2.0f; break;
	}
	textRect.w = textSize.x;
	textRect.h = textSize.y;
	return textRect;
}

UILabel::~UILabel() {
	TTF_DestroyText(textObject);
	textObject = nullptr;
}
