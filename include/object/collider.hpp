#pragma once

#include "../datatype/vector2.hpp"
#include "../geometry.hpp"
#include "gameObject.hpp"

namespace Bloodlust {

// A rectangular collider.
struct RectCollider;

// A circular collider.
struct CircleCollider;

// An abstract class representing a collider.
struct Collider : public GameObject {
  public:
	// The position of this collider.
	Vector2 position;

	// The velocity of this collider.
	Vector2 velocity = {0.0f, 0.0f};

	// True if this collider can interact with others, otherwise false.
	bool canCollide = true;

	// If true, this collider cannot be moved by gravity or other colliders.
	bool isFrozen = false;

  private:
	[[maybe_unused]]
	char _padding[6];

  public:
	// Steps this collider forward by a frame.
	virtual void step() = 0;

	// Returns true if this collider intersects with the given point.
	virtual bool intersects(const Vector2& other) const = 0;

	// Returns true if this collider intersects with the other.
	virtual bool intersects(const RectCollider& other) const = 0;

	// Returns true if this collider intersects with the other.
	virtual bool intersects(const CircleCollider& other) const = 0;

	virtual ~Collider() = default;
};

// A rectangular collider.
struct RectCollider : public Collider {
  public:
	// The size of this collider.
	Vector2 size;

	// Constructs a rectangular collider from a position and size.
	constexpr RectCollider(const Vector2& pos, const Vector2& size) : size(size) {
		position = pos;
	}

	// Steps this collider forward by a frame.
	void step() override;

	// Returns true if this collider intersects with the given point.
	constexpr bool intersects(const Vector2& other) const override {
		return rectIntersectsPoint(position, size, other);
	}

	// Returns true if this collider intersects with the other.
	constexpr bool intersects(const RectCollider& other) const override {
		return rectIntersectsRect(position, size, other.position, other.size);
	}

	// Returns true if this collider intersects with the other.
	constexpr bool intersects(const CircleCollider& other) const override;
};

// A circular collider.
struct CircleCollider : public Collider {
  public:
	// The radius of this collider.
	float radius;

  private:
	[[maybe_unused]]
	char _padding[4];

  public:
	// Constructs a circular collider from a position and radius.
	constexpr CircleCollider(const Vector2& pos, float radius) : radius(radius) {
		position = pos;
	}

	// Steps this collider forward by a frame.
	void step() override;

	// Returns true if this collider intersects with the given point.
	constexpr bool intersects(const Vector2& other) const override {
		return circleIntersectsPoint(position, radius, other);
	}

	// Returns true if this collider intersects with the other.
	constexpr bool intersects(const CircleCollider& other) const override {
		return circleIntersectsCircle(position, radius, other.position, other.radius);
	}

	// Returns true if this collider intersects with the other.
	constexpr bool intersects(const RectCollider& other) const override;
};

// Returns true if this collider intersects with the other.
constexpr bool RectCollider::intersects(const CircleCollider& other) const {
	return rectIntersectsCircle(position, size, other.position, other.radius);
}

// Returns true if this collider intersects with the other.
constexpr bool CircleCollider::intersects(const RectCollider& other) const {
	return other.intersects(*this);
}

}  // namespace Bloodlust
