#ifndef BL_DATA_COLLISIONGRID_H
#define BL_DATA_COLLISIONGRID_H

#include "../object/collider.hpp"

#include <memory>
#include <vector>

namespace Bloodlust::DataTypes {
using Objects::Collider;

class CollisionGrid {
	using GridCell = std::unique_ptr<std::vector<std::unique_ptr<Collider>>>;

  private:
	std::vector<GridCell> objects;

  public:
	const size_t width;
	const size_t height;
	const size_t gridSize;

	CollisionGrid(size_t width, size_t height, size_t gridSize = 64);

	const GridCell& getCell(size_t x, size_t y) const;

	~CollisionGrid();
};

}  // namespace Bloodlust::DataTypes

#endif
