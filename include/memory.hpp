#pragma once

#include <new>

// Returns a pointer to dynamically allocated memory of the given size. If the allocation fails,
// exitOOM() is called.
void* operator new(std::size_t size);

// Returns a pointer to dynamically allocated memory of the given size with the specified alignment.
// If the allocation fails, exitOOM() is called.
void* operator new(std::size_t size, std::align_val_t al);

// Returns a dynamically allocated array with the given size. If the allocation fails, exitOOM() is
// called.
void* operator new[](std::size_t size);

// Returns a dynamically allocated array with the specified size and alignment. If the allocation
// fails, exitOOM() is called.
void* operator new[](std::size_t size, std::align_val_t al);

// Deletes a pointer to dynamically allocated memory.
void operator delete(void* ptr) noexcept;

// Deletes a pointer to dynamically allocated memory of the given size.
void operator delete(void* ptr, std::size_t size) noexcept;

// Deletes a pointer to dynamically allocated memory with the specified alignment.
void operator delete(void* ptr, std::align_val_t al) noexcept;

// Deletes a pointer to dynamically allocated memory with the specified size and alignment.
void operator delete(void* ptr, std::size_t size, std::align_val_t al) noexcept;

// Deletes a dynamically allocated array.
void operator delete[](void* ptr) noexcept;

// Deletes a dynamically allocated array of the given size.
void operator delete[](void* ptr, std::size_t size) noexcept;

// Deletes an dynamically allocated array with the specified alignment.
void operator delete[](void* ptr, std::align_val_t al) noexcept;

// Deletes an dynamically allocated array with the specified size and alignment.
void operator delete[](void* ptr, std::size_t size, std::align_val_t al) noexcept;

namespace Bloodlust {

// Terminates the program, attempting to free reserved memory and printing an error message.
[[noreturn]]
void exitOOM();

}  // namespace Bloodlust
