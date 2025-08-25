#include "../../include/object/collider.hpp"

void Bloodlust::RectCollider::step() {
	position += velocity;
}

void Bloodlust::CircleCollider::step() {
	position += velocity;
}
