#pragma once

#include "../datatype/vector2.hpp"
#include "../util.hpp"
#include "collider.hpp"
#include "gameObject.hpp"

#include <atomic>
#include <cstdint>
#include <memory>

namespace Bloodlust {

// An abstract class representing an entity.
class Entity : public GameObject {
  private:
	// An atomic counter incremented for each entity created, responsible for assigning IDs.
	static std::atomic_uint64_t idCounter;

  protected:
	// A pointer to this entity's collider.
	std::unique_ptr<Collider> collider = nullptr;

  public:
	// The unique identifier of this entity.
	const std::uint64_t id;

  protected:
	// Initialises this entity.
	Entity();

  public:
	// Returns a reference to the pointer to this entity's collider.
	constexpr const std::unique_ptr<Collider>& getCollider() const noexcept {
		return collider;
	}

	// Sets the collider for this entity.
	inline void setCollider(Collider* newCollider) noexcept {
		collider = std::unique_ptr<Collider>(newCollider);
	}

	// Steps this entity forward by a frame.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool step() noexcept = 0;

	// Draws this entity.
	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool draw(std::uint64_t dt, const Vector2& cameraPos) noexcept = 0;

	virtual ~Entity() noexcept = default;
};

}  // namespace Bloodlust
