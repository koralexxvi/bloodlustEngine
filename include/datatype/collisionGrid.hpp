#pragma once

#include "../object/collider.hpp"
#include "point.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace Bloodlust {

// A class that represents a collision grid.
class CollisionGrid {
  public:
	using GridCell = std::unique_ptr<std::vector<std::unique_ptr<Collider>>>;

  private:
	// A map of unsigned 64-bit points to the cells this grid contains.
	std::unordered_map<Point<std::uint64_t>, GridCell, Point<std::uint64_t>::Hasher> cells;

  public:
	// The width of the collision grid, in cells.
	const std::uint64_t width;

	// The height of the collision grid, in cells.
	const std::uint64_t height;

	// The size of a grid cell.
	const std::uint64_t gridSize;

	// Constructs a collision grid given a width, height and grid size (defaulting to 64 units).
	CollisionGrid(std::uint64_t width, std::uint64_t height, std::uint64_t gridSize = 64) noexcept;

	// Returns a grid cell at the specified position. If the position is out of bounds, the
	// behaviour is undefined.
	const GridCell& getCell(std::uint64_t x, std::uint64_t y) const noexcept;
};

}  // namespace Bloodlust
