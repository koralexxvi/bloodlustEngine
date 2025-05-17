#include "../../include/object/uiLabel.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/font.hpp"
#include "../../include/graphics.hpp"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <string>

using Bloodlust::Objects::UILabel;

UILabel::UILabel(DataTypes::Font* initialFont) {
	TTF_Font* copy = TTF_CopyFont(initialFont->getData());
	if (copy != nullptr) {
		setFont(new DataTypes::Font(copy));
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
		if (!Graphics::drawRect(&drawRect)) {
			return false;
		}
	}

	if (textColor.a == 0) {
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
	return TTF_DrawRendererText(textObject.get(), drawRect.x, drawRect.y);
}

bool UILabel::isValid() const {
	return font != nullptr;
}

void UILabel::markDirty() {
	isDirty = 0b1111;
}

void UILabel::setFont(DataTypes::Font* newFont) {
	font = std::unique_ptr<DataTypes::Font>(newFont);
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

void UILabel::setTextColor(const DataTypes::Color& color) {
	textColor = color;
	isDirty |= 0b0001;
}

bool UILabel::update() {
	if (textObject == nullptr) {
		TTF_Text* newText = TTF_CreateText(app->textEngine, font->getData(), text.c_str(), 0);
		if (newText == nullptr) {
			return false;
		}
		textObject = std::unique_ptr<TTF_Text>(newText);
		isDirty &= 0b1011;
	}

	if (isDirty & 0b1000) {
		isDirty ^= TTF_SetTextFont(textObject.get(), font->getData()) << 3;
	}
	if (isDirty & 0b0100) {
		isDirty ^= TTF_SetTextString(textObject.get(), text.c_str(), 0) << 2;
	}
	if (isDirty & 0b0010) {
		isDirty ^= TTF_SetFontSize(font->getData(), cachedSize.y * textScale) << 1;
	}
	if (isDirty & 0b0001) {
		isDirty ^=
			TTF_SetTextColor(textObject.get(), textColor.r, textColor.g, textColor.b, textColor.a);
	}

	return isDirty == 0;
}
