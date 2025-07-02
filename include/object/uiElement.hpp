#ifndef BL_OBJ_UIELEMENT_H
#define BL_OBJ_UIELEMENT_H

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
	TOP,
	CENTER,
	BOTTOM
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
	int outlineWidth = 0;
	bool visible = true;

  private:
	uint8_t zIndex = 127;

	[[maybe_unused]]
	char _padding[2];

  public:
	Vector2 getDrawPos() const;
	Vector2 getDrawSize() const;
	SDL_FRect getDrawRect() const;

	uint8_t getZIndex() const;
	void setZIndex(uint8_t index);
	virtual bool setParent(UIElement* newParent);

	virtual bool canDraw() const = 0;
	virtual bool draw() = 0;

	virtual ~UIElement();

  protected:
	bool drawOutline() const;
	virtual bool onChildAdded(UIElement* child);
	virtual bool onChildRemoved(UIElement* child);
};

};	// namespace Bloodlust

#endif
