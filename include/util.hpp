#pragma once

#include <cstddef>
#include <functional>

#define BL_NODISCARD_SUCCESS_MSG "returned value reports success and should not be ignored"

namespace Bloodlust {

// Combines multiple hashes together.
// Derived from https://stackoverflow.com/a/66410378
template<typename T, typename... Rest>
constexpr void combineHashes(std::size_t& seed, const T& v, Rest&&... rest) {
	std::hash<T> hasher;
	seed ^= (hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
	int i[] = {0, (combineHashes(seed, std::forward<Rest>(rest)), 0)...};
	(void) (i);
}

}  // namespace Bloodlust
