#include "src/Entities/Entity/Entity.hpp"

Entity::Entity(std::size_t id) : id(id) {}

Entity::operator std::size_t() const {
    return this->id;
}

std::size_t Entity::getId() const {
    return this->id;
}

