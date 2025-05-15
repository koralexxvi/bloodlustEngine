#include "../../include/object/uiRect.hpp"

#include "../../include/datatype/vector2.hpp"
#include "../../include/graphics.hpp"

using Bloodlust::DataTypes::Vector2;
using Bloodlust::Objects::UIRect;

bool UIRect::canDraw() const {
	// TODO: frustum culling
	return visible && backgroundColor.a != 0;
}

bool UIRect::draw() {
	Vector2 drawPos = getDrawPos();
	Vector2 drawSize = getDrawSize();
	Graphics::setDrawColor(backgroundColor);
	return Graphics::fillRect(drawPos, drawSize);
}
