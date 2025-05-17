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
	sizeDirty = sizeDirty || (drawSize != cachedSize);
	if (isDirty) {
		if (sizeDirty) {
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
	isDirty = fontDirty = textDirty = sizeDirty = true;
}

void UILabel::setFont(DataTypes::Font* newFont) {
	font = std::unique_ptr<DataTypes::Font>(newFont);
	fontDirty = isDirty = true;
}

void UILabel::setTextScale(float newScale) {
	textScale = newScale;
	sizeDirty = isDirty = true;
}

void UILabel::setText(const std::string& str) {
	text = str;
	textDirty = isDirty = true;
}

void UILabel::setTextColor(const DataTypes::Color& color) {
	textColor = color;
	colorDirty = isDirty = true;
}

bool UILabel::update() {
	if (textObject == nullptr) {
		TTF_Text* newText = TTF_CreateText(app->textEngine, font->getData(), text.c_str(), 0);
		if (newText == nullptr) {
			return false;
		}
		textObject = std::unique_ptr<TTF_Text>(newText);
		textDirty = false;
	}

	if (fontDirty) {
		fontDirty = !TTF_SetTextFont(textObject.get(), font->getData());
	}
	if (textDirty) {
		textDirty = !TTF_SetTextString(textObject.get(), text.c_str(), 0);
	}
	if (sizeDirty) {
		sizeDirty = !TTF_SetFontSize(font->getData(), cachedSize.y * textScale);
	}
	if (colorDirty) {
		colorDirty =
			!TTF_SetTextColor(textObject.get(), textColor.r, textColor.g, textColor.b, textColor.a);
	}

	isDirty = false;
	return !(fontDirty || textDirty || sizeDirty || colorDirty);
}
