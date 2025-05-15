#include "../../include/object/uiLabel.hpp"

#include "../../include/app.hpp"
#include "../../include/datatype/font.hpp"
#include "../../include/graphics.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

using Bloodlust::Objects::UILabel;

bool UILabel::canDraw() const {
	return visible && (textColor.a != 0 || backgroundColor.a != 0);
}

bool UILabel::draw() {
	Vector2 drawPos = getDrawPos();
	Vector2 drawSize = getDrawSize();

	if (backgroundColor.a != 0) {
		Graphics::setDrawColor(backgroundColor);
		if (!Graphics::drawRect(drawPos, drawSize)) {
			return false;
		}
	}

	if (textColor.a == 0) {
		return true;
	}
	if (isDirty || drawSize != cachedSize) {
		cachedSize = drawSize;
		if (!update()) {
			return false;
		}
	}
	return Graphics::drawTexture(cachedText.get(), drawPos, drawSize);
}

void UILabel::setDirty() {
	isDirty = true;
}

void UILabel::setFont(DataTypes::Font* newFont) {
	font = newFont;
	isDirty = true;
}

void UILabel::setText(const std::string& str) {
	text = str;
	isDirty = true;
}

bool UILabel::update() {
	SDL_Surface* surface = TTF_RenderText_LCD(
		font->getData(),
		text.c_str(),
		0,
		textColor.toSDLColor(),
		backgroundColor.toSDLColor()
	);
	if (surface == nullptr) {
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
	SDL_DestroySurface(surface);
	if (texture == nullptr) {
		return false;
	}
	cachedText = std::unique_ptr<DataTypes::Texture>(new DataTypes::Texture(texture));
	isDirty = false;
	return true;
}
