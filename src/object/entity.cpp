#include "../../include/object/entity.hpp"

#include "../../include/object/collider.hpp"

using Bloodlust::Entity;

void Entity::setCollider(Collider* newCollider) {
	collider = std::unique_ptr<Collider>(newCollider);
}
