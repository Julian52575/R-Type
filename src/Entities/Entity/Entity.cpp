#include "./Entity.hpp"

Entity::Entity(uint32_t id) : id(id) {
    this->configId = 0;
}

Entity::Entity(uint32_t id, uint16_t configId) : id(id), configId(configId) {}

Entity::operator uint32_t() const {
    return this->id;
}

uint32_t Entity::getId() const {
    return this->id;
}

void Entity::setId(uint32_t id) {
    this->id = id;
}

uint16_t Entity::getConfigId() const {
    return this->configId;
}

void Entity::setConfigId(uint16_t configId) {
    this->configId = configId;
}
