#include "../include/world.hpp"
#include <cstdint>

using Bloodlust::World;

World::World(uint32_t width, uint32_t height) : width(width), height(height) {}

void World::addEntity(Entity* entity) {
	entities.insert(entity);
}

void World::removeEntity(Entity* entity) {
	entities.erase(entity);
}

Bloodlust::Camera* World::getCamera() const {
	return camera.get();
}

void World::setCamera(Camera* newCamera) {
	camera = std::unique_ptr<Camera>(newCamera);
}

Bloodlust::Vector2 World::getGravity() const {
	return gravity;
}

void World::setGravity(const Vector2& newGravity) {
	gravity = newGravity;
}

bool World::stepWorld() const {
	for (const auto& v : entities) {
		if (!v->step()) {
			return false;
		}
	}
	for (const auto& v : entities) {
		const auto& collider = v->getCollider();
		if (collider != nullptr && collider->canCollide) {
			collider->step();
		}
	}
	return true;
}

bool World::renderWorld(uint64_t dt) const {
	for (const auto& v : entities) {
		if (!v->draw(dt)) {
			return false;
		}
	}
	if (camera != nullptr) {
		camera->step(dt);
	}
	return true;
}
