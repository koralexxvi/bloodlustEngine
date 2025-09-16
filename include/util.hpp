#pragma once

#include <cstddef>
#include <functional>

#define BL_NODISCARD_SUCCESS_MSG "returned value reports success and should not be ignored"
#define BL_NODISCARD_LEAK_MSG \
	"returned value is not automatically freed and will cause a memory leak if ignored"

namespace Bloodlust {

// Combines multiple hashes together.
// Derived from https://stackoverflow.com/a/78509978
template<typename T, typename... Rest>
constexpr void combineHashes(std::size_t& seed, const T& v, Rest... rest) {
	constexpr std::size_t m = 0xe9846af9b1a615d;
	std::size_t x = seed + 0x9e3779b9 + std::hash<T>()(v);
	x ^= x >> 32;
	x *= m;
	x ^= x >> 32;
	x *= m;
	x ^= x >> 28;
	seed = x;
	combineHashes(seed, v, rest...);
}

}  // namespace Bloodlust
