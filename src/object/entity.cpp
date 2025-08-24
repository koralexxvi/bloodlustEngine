#include "../../include/object/entity.hpp"

#include "../../include/object/collider.hpp"

using Bloodlust::Entity;

const std::unique_ptr<Bloodlust::Collider>& Entity::getCollider() const {
	return collider;
}

void Entity::setCollider(Collider* newCollider) {
	collider = std::unique_ptr<Collider>(newCollider);
}
