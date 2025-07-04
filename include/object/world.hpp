#pragma once

#include "../datatype/raycastHit.hpp"
#include "../datatype/vector2.hpp"
#include "collider.hpp"
#include "gameObject.hpp"

#include <cstdint>
#include <list>
#include <memory>

namespace Bloodlust {

class World : public GameObject {
  private:
	Vector2 gravity = {0.0f, 196.2f};
	std::list<std::unique_ptr<Collider>> simulated;

  public:
	const uint16_t width;
	const uint16_t height;

  private:
	[[maybe_unused]]
	char _padding[4];

	World(uint16_t width, uint16_t height);

	Vector2 getGravity() const;
	void setGravity(const Vector2& newGravity);

	void stepPhysics();

	bool raycast(const Vector2& position, const Vector2& direction, RaycastHit* result) const;

	~World();
};

}  // namespace Bloodlust
