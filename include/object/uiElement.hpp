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
	UIPoint position = {};
	UIPoint size = {};
	Vector2 anchorPoint = {};
	UIElement* parent = nullptr;
	mutable Vector2 cachedPos;
	mutable Vector2 cachedSize;

  public:
	Color backgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};
	Color outlineColor = {0.0f, 0.0f, 0.0f, 0.0f};
	float outlineWidth = 0.0f;
	float zIndex = 0.5f;
	bool visible = true;

  protected:
	mutable bool posDirty = true;
	mutable bool sizeDirty = true;

  private:
	[[maybe_unused]]
	char _padding[5];

  protected:
	bool isOutlineVisible() const;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool onChildAdded(UIElement* child);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool onChildRemoved(UIElement* child);

  public:
	UIPoint getPosition() const;
	UIPoint getSize() const;
	Vector2 getAnchorPoint() const;

	void setPosition(const UIPoint& pos);
	void setSize(const UIPoint& size);
	void setAnchorPoint(const Vector2& anchor);

	Vector2 getDrawPos() const;
	Vector2 getDrawSize() const;
	SDL_FRect getDrawRect() const;

	virtual bool setParent(UIElement* newParent);

	virtual bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const = 0;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool draw() = 0;

	virtual ~UIElement();
};

};	// namespace Bloodlust
