#pragma once
#include <cstdint>
#include <queue>
#include <vector>
#include <iostream>

#include "../../Exceptions/ECSExecption.hpp"
#include "../Entity/Entity.hpp"

class EntityManager {
    public:
        explicit EntityManager(uint32_t maxEntities = 1000);
        ~EntityManager();
        Entity createEntity();
        void destroyEntity(Entity entity);
        bool isEntityActive(Entity entity) const;
        std::vector<Entity> getActiveEntities() const;
    private:
        std::queue<Entity> availableEntities;
        std::vector<bool> activeEntities;
};
