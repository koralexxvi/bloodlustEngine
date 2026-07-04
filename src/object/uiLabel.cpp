#include "../../include/object/uiLabel.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/font.hpp"
#include "../../include/geometry.hpp"
#include "../../include/logging.hpp"
#include "../../include/render/graphics.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

using Bloodlust::UILabel;

UILabel::UILabel(Font* initialFont) noexcept : font(initialFont) {
	textFont = font->getData(style, Font::alignHToTTFAlign(fontAlignH));
	textObject = TTF_CreateText(app->renderState.textEngine, textFont, text.c_str(), 0);
}

std::unique_ptr<UILabel> UILabel::copy() const noexcept {
	std::unique_ptr<UILabel> copy = std::make_unique<UILabel>(font);
	copy->setText(text);
	copy->setFontAlignH(fontAlignH);
	copy->setFontAlignV(fontAlignV);
	copy->setFontSize(fontSize);
	copy->setFontStyle(style);
	copy->textColour = textColour;
	return copy;
}

bool UILabel::isTextVisible() const noexcept {
	return textColour.a > 0.0f && (scaleText || fontSize > 0.0f);
}

bool UILabel::canDraw(const Vector2& clipPos, const Vector2& clipSize) const noexcept {
	if (!visible) {
		return false;
	}
	bool rectVisible = backgroundColour.a > 0.0f;
	bool outlineVisible = isOutlineVisible();
	bool textVisible = isTextVisible();
	if (!(rectVisible || outlineVisible || textVisible)) {
		return false;
	}
	SDL_FRect drawRect = getDrawRect();
	if (outlineVisible && outlineWidth > 0.0f) {
		float doubleWidth = outlineWidth * 2.0f;
		drawRect.x -= outlineWidth;
		drawRect.y -= outlineWidth;
		drawRect.w += doubleWidth;
		drawRect.h += doubleWidth;
	}
	return rectIntersectsRect(
		{drawRect.x, drawRect.y},
		{drawRect.w, drawRect.h},
		clipPos,
		clipSize
	);
}

bool UILabel::draw() noexcept {
	SDL_FRect drawRect = getDrawRect();
	if (drawRect.w == 0.0f || drawRect.h == 0.0f) {
		return true;
	}

	setDrawLayer(layer);
	if (backgroundColour.a > 0.0f) {
		setDrawColour(backgroundColour);
		fillRect(drawRect);
	}
	if (isOutlineVisible()) {
		setDrawColour(outlineColour);
		drawRectOutline(drawRect, outlineWidth);
	}

	if (!isTextVisible()) {
		return true;
	} else if (!updateText()) {
		return false;
	}

	float fontDrawSize;
	SDL_FRect textRect = getTextBounds(drawRect, fontDrawSize);

#ifdef BL_DEBUG_DRAW_TEXT_BACKGROUND
	setDrawColour({0.5f, 0.0f, 0.5f, 0.5f});
	fillRect({textRect.x, textRect.y - textRect.h, textRect.w, textRect.h});
#endif

	setDrawColour(textColour);
	drawText(textObject, {textRect.x, textRect.y}, fontDrawSize);
	return true;
}

void UILabel::setFont(Font* newFont) noexcept {
	if (newFont == nullptr) {
		setError("UILabel::setFont: passed font was null");
		return;
	}
	font = newFont;
	fontDirty = true;
	textBoundsDirty = true;
}

void UILabel::setText(const std::string& str) noexcept {
	text = str;
	TTF_SetTextString(textObject, str.c_str(), 0);
	textBoundsDirty = true;
}

SDL_FRect UILabel::getTextBounds(const SDL_FRect& drawRect, float& fontDrawSize) const noexcept {
	if (!textBoundsDirty && drawRect.x == lastDrawRect.x && drawRect.y == lastDrawRect.y
		&& drawRect.w == lastDrawRect.w && drawRect.h == lastDrawRect.h)
	{
		fontDrawSize = cachedFontDrawSize;
		return cachedTextBounds;
	}
	lastDrawRect = drawRect;

	float wrapWidth;
	if (wrapText) {
		float fontScale = fontSize / BL_FONT_SIZE;
		wrapWidth = drawRect.w;
		TTF_SetTextWrapWidth(textObject, static_cast<int>(SDL_ceilf(drawRect.w / fontScale)));
	} else {
		wrapWidth = 0.0f;
		TTF_SetTextWrapWidth(textObject, 0);
	}
	Vector2 stringSize = font->getStringSize(text.c_str(), textFont, fontSize, wrapWidth);
	if (scaleText) {
		float scaleX = drawRect.w / stringSize.x;
		float scaleY = drawRect.h / stringSize.y;
		float scale = scaleX < scaleY ? scaleX : scaleY;
		stringSize *= scale;
		fontDrawSize = fontSize * scale;
	} else {
		fontDrawSize = fontSize;
	}
	SDL_FRect textRect = {0.0f, 0.0f, stringSize.x, stringSize.y};
	switch (fontAlignH) {
		case AlignModeHorizontal::LEFT: textRect.x = drawRect.x; break;
		case AlignModeHorizontal::CENTER:
			textRect.x = drawRect.x + (drawRect.w - textRect.w) * 0.5f;
			break;
		case AlignModeHorizontal::RIGHT: textRect.x = (drawRect.x + drawRect.w) - textRect.w; break;
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

	cachedTextBounds = textRect;
	cachedFontDrawSize = fontDrawSize;
	textBoundsDirty = false;
	return textRect;
}

bool UILabel::updateText() noexcept {
	if (fontDirty) {
		textFont = font->getData(style, Font::alignHToTTFAlign(fontAlignH));
		if (textFont == nullptr || !TTF_SetTextFont(textObject, textFont)) {
			return false;
		}
		fontDirty = false;
	}
	return true;
}
