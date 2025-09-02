#pragma once

#include "../datatype/colour.hpp"
#include "../datatype/uiPoint.hpp"
#include "gameObject.hpp"

#include <SDL3/SDL_rect.h>
#include <unordered_set>

namespace Bloodlust {

// Represents a horizontal alignment.
enum class AlignModeHorizontal : std::int32_t {
	LEFT,	 // Left alignment.
	CENTER,	 // Centered alignment.
	RIGHT	 // Right alignment.
};

// Represents a vertical alignment.
enum class AlignModeVertical : std::int32_t {
	BOTTOM,	 // Bottom alignment.
	CENTER,	 // Centered alignment.
	TOP		 // Top alignment.
};

// Base class for UI elements.
class UIElement : public GameObject {
  protected:
	// An unordered set of pointers to this element's children.
	std::unordered_set<UIElement*> children = {};

	// The position of this element.
	UIPoint position = {};

	// The size of this element.
	UIPoint size = {};

	// The anchor point of this element.
	Vector2 anchorPoint = {};

	// The parent of this element.
	UIElement* parent = nullptr;

	// The cached draw position of this element.
	mutable Vector2 cachedPos;

	// The cached draw size of this element.
	mutable Vector2 cachedSize;

  public:
	// The background colour of this element.
	Colour backgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};

	// The outline colour of this element.
	Colour outlineColor = {0.0f, 0.0f, 0.0f, 0.0f};

	// The width of this element's outlines. 0.0f represents no outline, while positive values draw
	// outwards and negative values draw inward.
	float outlineWidth = 0.0f;

	// The z-index of this element.
	float zIndex = 0.5f;

	// Whether or not this element can be drawn.
	bool visible = true;

  protected:
	// True if cachedPos needs updating, otherwise false.
	mutable bool posDirty = true;

	// True if cachedSize needs updating, otherwise false.
	mutable bool sizeDirty = true;

  private:
	[[maybe_unused]]
	char _padding[5];

  protected:
	// Returns true if this element's outline can be drawn.
	bool isOutlineVisible() const;

	// Callback that is fired whenever a child is added.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool onChildAdded(UIElement* child);

	// Callback  that is fired whenever a child is removed.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool onChildRemoved(UIElement* child);

  public:
	// Returns the position of this element.
	UIPoint getPosition() const;

	// Returns the size of this element.
	UIPoint getSize() const;

	// Returns the anchor point of this element. <0.0f, 0.0f> represents bottom left, while
	// <1.0f, 1.0f> represents top right.
	Vector2 getAnchorPoint() const;

	// Sets the position of this element.
	void setPosition(const UIPoint& pos);

	// Sets the size of this element.
	void setSize(const UIPoint& size);

	// Sets the anchor point of this element.
	void setAnchorPoint(const Vector2& anchor);

	// Returns the draw position of this element. The returned value is cached for future calls.
	Vector2 getDrawPos() const;

	// Returns the draw size of this element. The returned value is cached for future calls.
	Vector2 getDrawSize() const;

	// Returns the draw position and size of this element as a rect.
	SDL_FRect getDrawRect() const;

	// Sets the parent of this element.
	virtual bool setParent(UIElement* newParent);

	// Returns if this element is able to be drawn.
	virtual bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const = 0;

	// Draws this element and returns true on success.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool draw() = 0;

	virtual ~UIElement();
};

};	// namespace Bloodlust
