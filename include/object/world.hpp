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
	const uint32_t width;
	const uint32_t height;

  private:
	World(uint32_t width, uint32_t height);

	Vector2 getGravity() const;
	void setGravity(const Vector2& newGravity);

	void stepPhysics();

	bool raycast(const Vector2& position, const Vector2& direction, RaycastHit* result) const;

	~World();
};

}  // namespace Bloodlust
