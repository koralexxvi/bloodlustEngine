#include "../../include/object/world.hpp"

#include <cstdint>

using Bloodlust::World;

World::World(uint32_t width, uint32_t height) : width(width), height(height) {}

Bloodlust::Vector2 World::getGravity() const {
	return gravity;
}

void World::setGravity(const Vector2& newGravity) {
	gravity = newGravity;
}

void World::stepPhysics() {
	for (auto& v : simulated) {
		if (v->canCollide) {
			v->step();
		}
	}
}
