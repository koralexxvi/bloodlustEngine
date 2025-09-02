#include "../../include/datatype/collisionGrid.hpp"

#include <memory>

using Bloodlust::CollisionGrid, Bloodlust::Collider;
using GridCell = std::unique_ptr<std::vector<std::unique_ptr<Collider>>>;

CollisionGrid::CollisionGrid(std::size_t width, std::size_t height, std::size_t gridSize)
	: width(width),
	  height(height),
	  gridSize(gridSize) {
	objects = std::vector<GridCell>(width * height);
}

const GridCell& CollisionGrid::getCell(std::size_t x, std::size_t y) const {
	return objects[x + (y * width)];
}

CollisionGrid::~CollisionGrid() {
	objects.clear();
}
