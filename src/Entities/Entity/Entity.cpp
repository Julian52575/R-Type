#include "./Entity.hpp"

Entity::Entity(uint32_t id) : id(id) {}

Entity::operator uint32_t() const {
    return this->id;
}

uint32_t Entity::getId() const {
    return this->id;
}

void Entity::setId(uint32_t id) {
    this->id = id;
}
