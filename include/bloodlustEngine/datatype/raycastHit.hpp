#pragma once

#include "vector2.hpp"

namespace Bloodlust {

// A structure that represents the result of a raycast.
struct RaycastHit {
	// The position of where the raycast hit.
	const Vector2 position;

	// The normal vector of the raycast hit.
	const Vector2 normal;
};

}  // namespace Bloodlust
