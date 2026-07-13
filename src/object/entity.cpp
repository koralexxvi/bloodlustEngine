#include <bloodlustEngine/object/entity.hpp>
#include <atomic>

std::atomic_uint64_t Bloodlust::Entity::idCounter = 0;

Bloodlust::Entity::Entity() : id(idCounter++) {}
