#include "../../include/datatype/collisionGrid.hpp"

#include <memory>

using Bloodlust::CollisionGrid, Bloodlust::Collider;
using GridCell = std::unique_ptr<std::vector<std::unique_ptr<Collider>>>;

CollisionGrid::CollisionGrid(size_t width, size_t height, size_t gridSize)
	: width(width),
	  height(height),
	  gridSize(gridSize) {
	objects = std::vector<GridCell>(width * height);
}

const GridCell& CollisionGrid::getCell(size_t x, size_t y) const {
	return objects[x + (y * width)];
}

CollisionGrid::~CollisionGrid() {
	objects.clear();
}
