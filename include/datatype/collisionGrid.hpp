#pragma once

#include "../object/collider.hpp"

#include <memory>
#include <vector>

namespace Bloodlust {

// A class that represents a collision grid.
class CollisionGrid {
  public:
	using GridCell = std::unique_ptr<std::vector<std::unique_ptr<Collider>>>;

  private:
	// A vector of the cells this grid contains.
	std::vector<GridCell> cells;

  public:
	// The width of the collision grid, in cells.
	const std::size_t width;

	// The height of the collision grid, in cells.
	const std::size_t height;

	// The size of a grid cell.
	const std::size_t gridSize;

	// Constructs a collision grid given a width, height and grid size (defaulting to 64 units).
	CollisionGrid(std::size_t width, std::size_t height, std::size_t gridSize = 64);

	// Returns a grid cell at the specified position. If the position is out of bounds, the
	// behaviour is undefined.
	const GridCell& getCell(std::size_t x, std::size_t y) const;

	// Destroys this collision grid. All of the grid's cells will be invalidated.
	~CollisionGrid();
};

}  // namespace Bloodlust
