#ifndef BL_OBJ_UIELEMENT_H
#define BL_OBJ_UIELEMENT_H

#include "../datatype/color.hpp"
#include "../datatype/uiPoint.hpp"
#include "gameObject.hpp"
#include "world.hpp"

#include <cstdint>
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
	DataTypes::Color backgroundColor = {255, 255, 255};
	bool visible = true;

  private:
	uint8_t _padding[3];

  protected:
	Vector2 getDrawPos() const;
	Vector2 getDrawSize() const;
	virtual bool onChildAdded(UIElement* child);
	virtual bool onChildRemoved(UIElement* child);

  public:
	UIElement();

	virtual bool setParent(UIElement* newParent);
	virtual bool canDraw() const = 0;
	virtual bool draw() = 0;

	virtual ~UIElement();
};

};	// namespace Bloodlust::Objects

#endif
