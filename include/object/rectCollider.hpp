#ifndef BL_OBJ_RECTCOLLIDER_H
#define BL_OBJ_RECTCOLLIDER_H

#include "collider.hpp"

namespace Bloodlust::Objects {

struct RectCollider : public Collider {
	Vector2 size;

	void step() override;

	bool intersects(const Vector2& other) const override;
	bool intersects(const RectCollider& other) const override;
	bool intersects(const CircleCollider& other) const override;
};

}  // namespace Bloodlust::Objects

#endif
