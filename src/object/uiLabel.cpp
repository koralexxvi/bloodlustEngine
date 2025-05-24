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
	return visible && (textColor.a != 0 || backgroundColor.a != 0);
}

bool UILabel::draw() {
	SDL_FRect drawRect = getDrawRect();
	if (drawRect.w == 0.0f || drawRect.h == 0.0f) {
		return true;
	}

	if (backgroundColor.a != 0) {
		Graphics::setDrawColor(backgroundColor);
		if (!Graphics::fillRect(&drawRect)) {
			return false;
		}
	}

	if (textColor.a == 0) {
		return true;
	}

	Vector2 drawSize = {drawRect.w, drawRect.h};
	if (drawSize != cachedSize) {
		isDirty |= 0b00010;
	}
	if (isDirty != 0) {
		if (isDirty & 0b00010) {
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
	isDirty = 0b11111;
}

void UILabel::setFont(Font* newFont) {
	font = std::unique_ptr<Font>(newFont);
	isDirty |= 0b10000;
}

void UILabel::setFontSize(float newSize) {
	fontSize = newSize;
	isDirty |= 0b00010;
}

void UILabel::setFontAlignHorizontal(AlignModeHorizontal align) {
	alignH = align;
	isDirty |= 0b01000;
}

void UILabel::setFontAlignVertical(AlignModeVertical align) {
	alignV = align;
}

void UILabel::setText(const std::string& str) {
	text = str;
	isDirty |= 0b00100;
}

void UILabel::setTextColor(const Color& color) {
	textColor = color;
	isDirty |= 0b00001;
}

bool UILabel::update() {
	if (textObject == nullptr) {
		TTF_Text* newText = TTF_CreateText(app->textEngine, font->getData(), text.c_str(), 0);
		if (newText == nullptr) {
			return false;
		}
		TTF_DestroyText(textObject);
		textObject = newText;
		isDirty &= ~0b10100;
	}

	if (isDirty & 0b10000) {
		isDirty ^= TTF_SetTextFont(textObject, font->getData()) << 4;
	}
	if (isDirty & 0b01000) {
		TTF_HorizontalAlignment alignment;
		switch (alignH) {
			case AlignModeHorizontal::LEFT: alignment = TTF_HORIZONTAL_ALIGN_LEFT; break;
			case AlignModeHorizontal::CENTER: alignment = TTF_HORIZONTAL_ALIGN_CENTER; break;
			case AlignModeHorizontal::RIGHT: alignment = TTF_HORIZONTAL_ALIGN_RIGHT; break;
			default: return false;
		}
		TTF_SetFontWrapAlignment(font->getData(), alignment);
		isDirty ^= 0b01000;
	}
	if (isDirty & 0b00100) {
		isDirty ^= TTF_SetTextString(textObject, text.c_str(), 0) << 2;
	}
	if (isDirty & 0b00010) {
		if (fontSize > 0.0f) {
			isDirty ^= TTF_SetFontSize(font->getData(), fontSize) << 1;
		} else {
			isDirty ^= TTF_SetFontSize(font->getData(), cachedSize.y) << 1;
		}
	}
	if (isDirty & 0b00001) {
		isDirty ^= TTF_SetTextColor(textObject, textColor.r, textColor.g, textColor.b, textColor.a);
	}

	int w, h;
	if (!TTF_GetTextSize(textObject, &w, &h)) {
		return false;
	}
	textSize = {float(w), float(h)};

	return isDirty == 0;
}

SDL_FRect UILabel::getTextBounds(const SDL_FRect& drawRect) const {
	SDL_FRect textRect = {0.0f, 0.0f, textSize.x, textSize.y};
	switch (alignH) {
		case AlignModeHorizontal::LEFT: textRect.x = drawRect.x; break;
		case AlignModeHorizontal::RIGHT: textRect.x = (drawRect.x + drawRect.w) - textSize.x; break;
		default: textRect.x = 0.0f; break;
	}
	switch (alignV) {
		case AlignModeVertical::TOP: textRect.y = drawRect.y; break;
		case AlignModeVertical::BOTTOM: textRect.y = drawRect.y + (drawRect.h - textSize.y); break;
		default: textRect.y = drawRect.y + (drawRect.h - textSize.y) / 2.0f; break;
	}
	return textRect;
}

UILabel::~UILabel() {
	TTF_DestroyText(textObject);
	textObject = nullptr;
}
