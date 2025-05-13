#ifndef BL_OBJ_CIRCLECOLLIDER_H
#define BL_OBJ_CIRCLECOLLIDER_H

#include "collider.hpp"

namespace Bloodlust::Objects {

struct CircleCollider : Collider {
	float radius;

	void step() override;

	bool intersects(const Vector2& other) const override;
	bool intersects(const RectCollider& other) const override;
	bool intersects(const CircleCollider& other) const override;
};

}  // namespace Bloodlust::Objects

#endif
