#include "../include/world.hpp"

#include <cstdint>

using Bloodlust::World;

World::World(std::size_t width, std::size_t height) noexcept
	: grid(width, height),
	  width(width),
	  height(height) {}

void World::addEntity(Entity* entity) noexcept {
	entities.insert(entity);
}

void World::removeEntity(Entity* entity) noexcept {
	entities.erase(entity);
}

bool World::stepWorld() const noexcept {
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

bool World::renderWorld(std::uint64_t dt) const noexcept {
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
