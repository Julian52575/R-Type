#pragma once
#include <cstdint>
#include <queue>
#include <vector>
#include <iostream>

#include "../../Exceptions/ECSExecption.hpp"
#include "../Entity/Entity.hpp"

class EntityManager {
    public:
        explicit EntityManager(uint32_t maxEntities = 10000);
        ~EntityManager();
        Entity& createEntity();
        void destroyEntity(Entity entity);
        bool isEntityActive(Entity entity) const;
        std::vector<Entity> getActiveEntities() const;

    private:
        std::queue<Entity> availableEntities;
        std::vector<std::pair<Entity, bool>> activeEntities;
};
