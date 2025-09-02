#include "../include/memory.hpp"

#include "../include/logging.hpp"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <exception>

namespace {

void* _emem = SDL_malloc(4096);

}

[[noreturn]]
void Bloodlust::exitOOM() {
	SDL_free(_emem);
	log(SDL_LOG_PRIORITY_CRITICAL, "Memory allocation failure");
	std::terminate();
}

#define BL_IMPLEMENT_NEW(signature)   \
	signature {                       \
		void* ptr = SDL_malloc(size); \
		if (ptr == nullptr) {         \
			Bloodlust::exitOOM();     \
		}                             \
		return ptr;                   \
	}

#define BL_IMPLEMENT_NEW_ALIGNED(signature)                   \
	signature {                                               \
		void* ptr = SDL_aligned_alloc(std::size_t(al), size); \
		if (ptr == nullptr) {                                 \
			Bloodlust::exitOOM();                             \
		}                                                     \
		return ptr;                                           \
	}

#define BL_IMPLEMENT_DELETE(funcSig, delFunc) \
	funcSig {                                 \
		delFunc(ptr);                         \
	}

BL_IMPLEMENT_NEW(void* operator new(std::size_t size))
BL_IMPLEMENT_NEW_ALIGNED(void* operator new(std::size_t size, std::align_val_t al))
BL_IMPLEMENT_NEW(void* operator new[](std::size_t size))
BL_IMPLEMENT_NEW_ALIGNED(void* operator new[](std::size_t size, std::align_val_t al))

BL_IMPLEMENT_DELETE(void operator delete(void* ptr), SDL_free)
BL_IMPLEMENT_DELETE(void operator delete(void* ptr, std::size_t), SDL_free)
BL_IMPLEMENT_DELETE(void operator delete(void* ptr, std::align_val_t), SDL_aligned_free)
BL_IMPLEMENT_DELETE(
	void
	operator delete(void* ptr, std::size_t, std::align_val_t),
	SDL_aligned_free
)

BL_IMPLEMENT_DELETE(void operator delete[](void* ptr), SDL_free)
BL_IMPLEMENT_DELETE(void operator delete[](void* ptr, std::size_t), SDL_free)
BL_IMPLEMENT_DELETE(void operator delete[](void* ptr, std::align_val_t), SDL_aligned_free)
BL_IMPLEMENT_DELETE(
	void
	operator delete[](void* ptr, std::size_t, std::align_val_t),
	SDL_aligned_free
)

#undef BL_IMPLEMENT_NEW
#undef BL_IMPLEMENT_NEW_ALIGNED
#undef BL_IMPLEMENT_DELETE
