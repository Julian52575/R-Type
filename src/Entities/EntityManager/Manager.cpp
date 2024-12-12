#include "./Manager.hpp"
#include <vector>

EntityManager::EntityManager(uint32_t maxEntities){
    // this->activeEntities.resize(maxEntities, false);
    
    for (uint32_t i = 0; i < maxEntities; ++i) {
        Entity entity(i);
        this->availableEntities.push(entity);
        this->activeEntities.push_back(std::make_pair(entity, false));
    }
}

EntityManager::~EntityManager() {}

Entity& EntityManager::createEntity(){
    if (this->availableEntities.empty()) {
        throw ECSException("No more entities available.");
    }
    Entity& newEntity = this->availableEntities.front();

    this->availableEntities.pop();
    this->activeEntities[newEntity].second = true;
    return newEntity;
}

void EntityManager::destroyEntity(Entity entity){
    if (entity >= this->activeEntities.size() || !this->activeEntities[entity].second) {
        throw ECSException("Invalid entity or entity already destroyed.");
    }
    this->activeEntities[entity].second = false;
    this->availableEntities.push(entity);
}

bool EntityManager::isEntityActive(Entity entity) const {
    if (entity >= this->activeEntities.size()) {
        throw ECSException("Invalid entity.");
    }
    return this->activeEntities[entity].second;
}

std::vector<Entity> EntityManager::getActiveEntities() const {
    std::vector<Entity> entities;
    for (uint32_t i = 0; i < this->activeEntities.size(); ++i) {
        if (this->activeEntities[i].second) {
            entities.push_back(this->activeEntities[i].first);
        }
    }
    return entities;
}

