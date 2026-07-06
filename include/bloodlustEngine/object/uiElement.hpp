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

	// True if cachedPos needs updating, otherwise false.
	mutable bool posDirty = true;

	// True if cachedSize needs updating, otherwise false.
	mutable bool sizeDirty = true;

  public:
	// The background colour of this element.
	Colour backgroundColour = {0.0f, 0.0f, 0.0f, 0.0f};

	// The outline colour of this element.
	Colour outlineColour = {0.0f, 0.0f, 0.0f, 0.0f};

	// The layer of this element. If this is greater than or equal to BL_LAYER_COUNT, the behaviour
	// is undefined.
	std::size_t layer = 0;

	// The width of this element's outlines. 0.0f represents no outline, while positive values draw
	// outwards and negative values draw inward.
	float outlineWidth = 0.0f;

	// Whether or not this element can be drawn.
	bool visible = true;

  protected:
	// Returns true if this element's outline can be drawn.
	bool isOutlineVisible() const noexcept;

	// Callback that is fired whenever a child is added.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool onChildAdded(UIElement* child) noexcept;

	// Callback  that is fired whenever a child is removed.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool onChildRemoved(UIElement* child) noexcept;

  public:
	// Returns the position of this element.
	constexpr UIPoint getPosition() const noexcept {
		return position;
	}

	// Returns the size of this element.
	constexpr UIPoint getSize() const noexcept {
		return size;
	}

	// Returns the anchor point of this element. <0.0f, 0.0f> represents bottom left, while
	// <1.0f, 1.0f> represents top right.
	constexpr Vector2 getAnchorPoint() const noexcept {
		return anchorPoint;
	}

	// Sets the position of this element.
	inline void setPosition(const UIPoint& newPos) noexcept {
		position = newPos;
		posDirty = true;
	}

	// Sets the size of this element.
	inline void setSize(const UIPoint& newSize) noexcept {
		size = newSize;
		sizeDirty = true;
	}

	// Sets the anchor point of this element.
	inline void setAnchorPoint(const Vector2& newAnchor) noexcept {
		anchorPoint = newAnchor;
		posDirty = true;
	}

	// Returns the draw position of this element. The returned value is cached for future calls.
	constexpr Vector2 getDrawPos() const noexcept {
		if (!posDirty) {
			return cachedPos;
		}
		Vector2 relPos;
		if (parent != nullptr) {
			relPos = parent->getDrawPos() + position.toAbsolutePos(parent->getDrawSize());
		} else {
			relPos = position.toAbsolutePos();
		}
		relPos -= anchorPoint * getDrawSize();
		posDirty = false;
		cachedPos = relPos;
		return relPos;
	}

	// Returns the draw size of this element. The returned value is cached for future calls.
	constexpr Vector2 getDrawSize() const noexcept {
		if (!sizeDirty) {
			return cachedSize;
		}
		Vector2 resolution = app->window->getSize().toVector();
		float relativeX = resolution.x * size.xRel;
		float relativeY = resolution.y * size.yRel;
		std::int32_t xOffset = size.xOffset;
		std::int32_t yOffset = size.yOffset;
		for (UIElement* p = parent; p != nullptr; p = p->parent) {
			relativeX *= p->size.xRel;
			relativeY *= p->size.yRel;
			xOffset += p->size.xOffset;
			yOffset += p->size.yOffset;
		}
		Vector2 size = {
			relativeX + static_cast<float>(xOffset),
			relativeY + static_cast<float>(yOffset)
		};
		sizeDirty = false;
		cachedSize = size;
		return size;
	}

	// Returns the draw position and size of this element as a rect.
	constexpr SDL_FRect getDrawRect() const noexcept {
		Vector2 absPos = getDrawPos();
		Vector2 absSize = getDrawSize();
		return {absPos.x, absPos.y, absSize.x, absSize.y};
	}

	// Sets the parent of this element.
	virtual bool setParent(UIElement* newParent) noexcept;

	// Returns if this element is able to be drawn.
	virtual bool canDraw(const Vector2& clipPos, const Vector2& clipSize) const noexcept = 0;

	// Draws this element. Will throw InternalError on failure.
	virtual void draw() = 0;

	virtual ~UIElement() noexcept;
};

};	// namespace Bloodlust
