#include "../../include/datatype/collisionGrid.hpp"

using Bloodlust::CollisionGrid;

CollisionGrid::CollisionGrid(std::size_t width, std::size_t height, std::size_t gridSize)
	: width(width),
	  height(height),
	  gridSize(gridSize) {
	cells = std::vector<GridCell>(width * height);
}

const CollisionGrid::GridCell& CollisionGrid::getCell(std::size_t x, std::size_t y) const {
	return cells[x + (y * width)];
}

CollisionGrid::~CollisionGrid() {
	cells.clear();
}
