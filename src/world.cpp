#include <bloodlustEngine/object/entity.hpp>
#include <bloodlustEngine/world.hpp>
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

void World::stepWorld() const {
	for (const auto& v : entities) {
		v->step();
	}
	for (const auto& v : entities) {
		const auto& collider = v->getCollider();
		if (collider != nullptr && collider->canCollide) {
			collider->step();
		}
	}
}

void World::renderWorld(std::uint64_t dt) const {
	Vector2 cameraPos;
	if (camera != nullptr) {
		camera->step(dt);
		cameraPos = camera->getPosition();
	} else {
		cameraPos = {};
	}
	for (const auto& v : entities) {
		v->draw(dt, cameraPos);
	}
}
