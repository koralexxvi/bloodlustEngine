#include "../../include/datatype/collisionGrid.hpp"

#include <SDL3/SDL_assert.h>
#include <unordered_map>

using Bloodlust::CollisionGrid;

CollisionGrid::CollisionGrid(std::uint64_t width, std::uint64_t height, std::uint64_t gridSize)
	: width(width),
	  height(height),
	  gridSize(gridSize) {
	cells = std::unordered_map<Point<std::uint64_t>, GridCell, Point<std::uint64_t>::Hasher>(
		(width + height) / gridSize
	);
}

const CollisionGrid::GridCell& CollisionGrid::getCell(std::uint64_t x, std::uint64_t y) const {
	SDL_assert(x < width && y < width);
	return cells.at({x, y});
}
