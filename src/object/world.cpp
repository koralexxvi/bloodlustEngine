#include "../../include/object/world.hpp"

#include <cstdint>

using Bloodlust::Objects::World;
using Bloodlust::DataTypes::Vector2;

World::World(uint16_t width, uint16_t height) : width(width), height(height) {}

Vector2 World::getGravity() const {
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
