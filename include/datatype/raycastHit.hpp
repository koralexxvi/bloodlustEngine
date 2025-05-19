#ifndef BL_DATA_RAYCASTHIT_H
#define BL_DATA_RAYCASTHIT_H

#include "vector2.hpp"

namespace Bloodlust {

struct RaycastHit {
	const Vector2 position;
	const Vector2 normal;
};

}  // namespace Bloodlust

#endif
