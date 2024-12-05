#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include <vector>
#include "../../Entities/Entity/Entity.hpp"

namespace System {
    class Lifetime {
        public:
            Lifetime();
            ~Lifetime();
            std::vector<Entity> update(sparse_array<Component::LifeTime>& lifetimes,float deltaTime);
    };
}
