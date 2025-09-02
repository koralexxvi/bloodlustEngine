#pragma once

#include <new>

void* operator new(std::size_t size);
void* operator new(std::size_t size, std::align_val_t al);
void* operator new[](std::size_t size);
void* operator new[](std::size_t size, std::align_val_t al);

void operator delete(void* ptr);
void operator delete(void* ptr, std::size_t size);
void operator delete(void* ptr, std::align_val_t al);
void operator delete(void* ptr, std::size_t size, std::align_val_t al);

void operator delete[](void* ptr);
void operator delete[](void* ptr, std::size_t size);
void operator delete[](void* ptr, std::align_val_t al);
void operator delete[](void* ptr, std::size_t size, std::align_val_t al);

namespace Bloodlust {

[[noreturn]]
void exitOOM();

}  // namespace Bloodlust
