#pragma once

#include "../datatype/color.hpp"
#include "../datatype/uiPoint.hpp"
#include "gameObject.hpp"

#include <SDL3/SDL_rect.h>
#include <unordered_set>

namespace Bloodlust {

enum class AlignModeHorizontal {
	LEFT,
	CENTER,
	RIGHT
};

enum class AlignModeVertical {
	BOTTOM,
	CENTER,
	TOP
};

class UIElement : public GameObject {
  protected:
	std::unordered_set<UIElement*> children = {};
	UIElement* parent = nullptr;

  public:
	UIPoint position = {};
	UIPoint size = {};
	Color backgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};
	Color outlineColor = {0.0f, 0.0f, 0.0f, 0.0f};
	Vector2 anchorPoint = {};
	float outlineWidth = 0.0f;
	bool visible = true;
	uint8_t zIndex = 127;

  private:
	[[maybe_unused]]
	char _padding[2];

  protected:
	virtual bool onChildAdded(UIElement* child);
	virtual bool onChildRemoved(UIElement* child);

  public:
	Vector2 getDrawPos() const;
	Vector2 getDrawSize() const;
	SDL_FRect getDrawRect() const;

	virtual bool setParent(UIElement* newParent);

	virtual bool canDraw() const = 0;
	virtual bool draw() = 0;

	virtual ~UIElement();
};

};	// namespace Bloodlust
