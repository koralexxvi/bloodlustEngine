#pragma once

#include "../datatype/vector2.hpp"
#include "../geometry.hpp"
#include "gameObject.hpp"

namespace Bloodlust {

struct RectCollider;
struct CircleCollider;

struct Collider : public GameObject {
	Vector2 position;
	Vector2 velocity = {0.0f, 0.0f};

	bool canCollide = true;
	bool isFrozen = false;

  private:
	[[maybe_unused]]
	char _padding[6];

  public:
	virtual void step() = 0;

	virtual bool intersects(const Vector2& other) const = 0;
	virtual bool intersects(const RectCollider& other) const = 0;
	virtual bool intersects(const CircleCollider& other) const = 0;

	virtual ~Collider() = default;
};

struct RectCollider : public Collider {
	Vector2 size;

	void step() override;

	constexpr bool intersects(const Vector2& other) const override {
		return rectIntersectsPoint(position, size, other);
	}

	constexpr bool intersects(const RectCollider& other) const override {
		return rectIntersectsRect(position, size, other.position, other.size);
	}

	constexpr bool intersects(const CircleCollider& other) const override;
};

struct CircleCollider : public Collider {
	float radius;

  private:
	[[maybe_unused]]
	char _padding[4];

  public:
	void step() override;

	constexpr bool intersects(const Vector2& other) const override {
		return circleIntersectsPoint(position, radius, other);
	}

	constexpr bool intersects(const CircleCollider& other) const override {
		return circleIntersectsCircle(position, radius, other.position, other.radius);
	}

	constexpr bool intersects(const RectCollider& other) const override;
};

constexpr bool RectCollider::intersects(const CircleCollider& other) const {
	return rectIntersectsCircle(position, size, other.position, other.radius);
}

constexpr bool CircleCollider::intersects(const RectCollider& other) const {
	return other.intersects(*this);
}

}  // namespace Bloodlust
