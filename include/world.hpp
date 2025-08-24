#pragma once

#include "camera.hpp"
#include "datatype/raycastHit.hpp"
#include "datatype/vector2.hpp"
#include "object/entity.hpp"

#include <cstdint>
#include <memory>
#include <unordered_set>

namespace Bloodlust {

class World {
  private:
	std::unordered_set<Entity*> entities = {};
	std::unique_ptr<Camera> camera = nullptr;
	Vector2 gravity = {0.0f, 196.2f};

  public:
	const uint32_t width;
	const uint32_t height;

	World(uint32_t width, uint32_t height);

	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);

	Vector2 getGravity() const;
	void setGravity(const Vector2& newGravity);

	bool stepWorld() const;
	bool stepRender(uint64_t dt) const;

	bool raycast(const Vector2& position, const Vector2& direction, RaycastHit& result) const;

	~World();
};

}  // namespace Bloodlust
