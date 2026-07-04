#include "../../include/object/collider.hpp"

void Bloodlust::RectCollider::step() noexcept {
	position += velocity;
}

void Bloodlust::CircleCollider::step() noexcept {
	position += velocity;
}
