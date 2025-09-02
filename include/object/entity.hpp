#pragma once

#include "../util.hpp"
#include "collider.hpp"
#include "gameObject.hpp"

#include <cstdint>
#include <memory>

namespace Bloodlust {

// An abstract class representing an entity.
class Entity : public GameObject {
  protected:
	// A pointer to this entity's collider.
	std::unique_ptr<Collider> collider = nullptr;

  public:
	// Returns a reference to the pointer to this entity's collider.
	const std::unique_ptr<Collider>& getCollider() const;

	// Sets the collider for this entity.
	void setCollider(Collider* newCollider);

	// Steps this entity forward by a frame.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool step() = 0;

	// Draws this entity.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool draw(std::uint64_t dt) = 0;

	virtual ~Entity() = default;
};

}  // namespace Bloodlust
