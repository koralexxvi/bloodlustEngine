#include "../include/world.hpp"

#include <cstdint>

using Bloodlust::World;

World::World(std::size_t width, std::size_t height)
	: grid(width, height),
	  width(width),
	  height(height) {}

void World::addEntity(Entity* entity) {
	entities.insert(entity);
}

void World::removeEntity(Entity* entity) {
	entities.erase(entity);
}

void World::setCamera(Camera* newCamera) {
	camera = std::unique_ptr<Camera>(newCamera);
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

bool World::renderWorld(std::uint64_t dt) const {
	Vector2 cameraPos;
	if (camera != nullptr) {
		camera->step(dt);
		cameraPos = camera->getPosition();
	} else {
		cameraPos = {};
	}
	for (const auto& v : entities) {
		if (!v->draw(dt, cameraPos)) {
			return false;
		}
	}
	return true;
}
