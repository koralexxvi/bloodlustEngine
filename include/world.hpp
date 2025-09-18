#pragma once

#include "camera.hpp"
#include "datatype/collisionGrid.hpp"
#include "datatype/raycastHit.hpp"
#include "datatype/vector2.hpp"
#include "object/entity.hpp"

#include <cstdint>
#include <memory>
#include <unordered_set>

namespace Bloodlust {

// A class that handles simulation and rendering of the entities it contains.
class World {
  private:
	// The world's collision grid.
	CollisionGrid grid;

	// An unordered set of pointers to currently simulated entities.
	std::unordered_set<Entity*> entities = {};

	// A pointer to the active camera. If null, no entities will be rendered.
	std::unique_ptr<Camera> camera = nullptr;

	// The current gravity of the world.
	Vector2 gravity = {0.0f, 196.2f};

  public:
	// The width of the world.
	const std::size_t width;

	// The height of the world.
	const std::size_t height;

	// Creates a new world from a width and height.
	World(std::size_t width, std::size_t height);

	// Adds the entity to the simulation.
	void addEntity(Entity* entity);

	// Removes the entity from the simulation.
	void removeEntity(Entity* entity);

	// Gets the currently active camera. The returned pointer may be null.
	constexpr Camera* getCamera() const {
		return camera.get();
	}

	// Sets the currently active camera. May be null.
	void setCamera(Camera* newCamera);

	// Gets the world's current gravity.
	constexpr Vector2 getGravity() const {
		return gravity;
	}

	// Sets the world's current gravity.
	void setGravity(const Vector2& newGravity);

	// Performs a physics step on the world.
	bool stepWorld() const;

	// Queues each entity in the world to be drawn.
	bool renderWorld(std::uint64_t dt) const;

	// Performs a raycast in the world, returning true if it hit something and fills result with its
	// details. If nothing was hit, then the value of result is unchanged.
	bool raycast(const Vector2& position, const Vector2& direction, RaycastHit& result) const;
};

}  // namespace Bloodlust
