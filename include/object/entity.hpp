#pragma once

#include "../util.hpp"
#include "collider.hpp"
#include "gameObject.hpp"

#include <cstdint>
#include <memory>

namespace Bloodlust {

class Entity : public GameObject {
  protected:
	std::unique_ptr<Collider> collider = nullptr;

  public:
	const std::unique_ptr<Collider>& getCollider() const;
	void setCollider(Collider* newCollider);

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool step() = 0;

	[[nodiscard(BL_NODISCARD_SUCCESS_MSG)]]
	virtual bool draw(uint64_t dt) = 0;

	virtual ~Entity() = default;
};

}  // namespace Bloodlust
