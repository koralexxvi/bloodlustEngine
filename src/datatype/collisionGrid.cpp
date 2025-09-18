#include "../../include/datatype/collisionGrid.hpp"

#include <unordered_map>

using Bloodlust::CollisionGrid;

CollisionGrid::CollisionGrid(std::size_t width, std::size_t height, std::size_t gridSize)
	: width(width),
	  height(height),
	  gridSize(gridSize) {
	cells = std::unordered_map<Point<std::size_t>, GridCell, Point<std::size_t>::Hasher>(
		(width + height) / gridSize
	);
}

const CollisionGrid::GridCell& CollisionGrid::getCell(std::size_t x, std::size_t y) const {
	return cells.at({x, y});
}

CollisionGrid::~CollisionGrid() {
	cells.clear();
}
