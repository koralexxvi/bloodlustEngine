#ifndef BL_OBJ_COLLIDER_H
#define BL_OBJ_COLLIDER_H

#include "../datatype/vector2.hpp"
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

}  // namespace Bloodlust

#endif
