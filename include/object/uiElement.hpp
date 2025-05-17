#ifndef BL_OBJ_UIELEMENT_H
#define BL_OBJ_UIELEMENT_H

#include "../datatype/color.hpp"
#include "../datatype/uiPoint.hpp"
#include "gameObject.hpp"
#include "world.hpp"

#include <SDL3/SDL_rect.h>
#include <unordered_set>

namespace Bloodlust::Objects {

class UIElement : GameObject {
  protected:
	std::unordered_set<UIElement*> children = {};
	UIElement* parent = nullptr;

  public:
	DataTypes::UIPoint position = {};
	DataTypes::UIPoint size = {};
	DataTypes::Vector2 anchorPoint = {};
	DataTypes::Color backgroundColor = {64, 64, 64};
	bool visible = true;

  private:
	uint8_t zIndex = 0;

	[[maybe_unused]]
	char _padding[2];

  public:
	UIElement();

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
	virtual bool onChildAdded(UIElement* child);
	virtual bool onChildRemoved(UIElement* child);
};

};	// namespace Bloodlust::Objects

#endif
