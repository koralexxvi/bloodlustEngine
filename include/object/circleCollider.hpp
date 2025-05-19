#ifndef BL_OBJ_CIRCLECOLLIDER_H
#define BL_OBJ_CIRCLECOLLIDER_H

#include "collider.hpp"

namespace Bloodlust {

struct CircleCollider : public Collider {
	float radius;

  private:
	[[maybe_unused]]
	char _padding[4];

  public:
	void step() override;

	bool intersects(const Vector2& other) const override;
	bool intersects(const RectCollider& other) const override;
	bool intersects(const CircleCollider& other) const override;
};

}  // namespace Bloodlust

#endif
