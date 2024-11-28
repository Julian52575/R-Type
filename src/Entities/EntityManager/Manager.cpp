#include "Manager.hpp"

EntityManager::EntityManager(std::size_t maxEntities){
    this->activeEntities.resize(maxEntities, false);
    for (size_t i = 0; i < maxEntities; ++i) {
        this->availableEntities.push(Entity(i));
    }
}

EntityManager::~EntityManager(){};

Entity EntityManager::createEntity(){
    if (this->availableEntities.empty()) {
        throw ECSException("No more entities available.");
    }
    Entity newEntity = this->availableEntities.front();
    this->availableEntities.pop();
    this->activeEntities[newEntity] = true;
    return newEntity;
}

void EntityManager::destroyEntity(Entity entity){
    if (entity >= this->activeEntities.size() || !this->activeEntities[entity]) {
        throw ECSException("Invalid entity or entity already destroyed.");
    }
    this->activeEntities[entity] = false;
    this->availableEntities.push(entity);
}

bool EntityManager::isEntityActive(Entity entity) const {
    if (entity >= this->activeEntities.size()) {
        throw ECSException("Invalid entity.");
    }
    return this->activeEntities[entity];
}

std::vector<Entity> EntityManager::getActiveEntities() const {
    std::vector<Entity> entities;
    for (size_t i = 0; i < this->activeEntities.size(); ++i) {
        if (this->activeEntities[i]) {
            entities.push_back(Entity(i));
        }
    }
    return entities;
}