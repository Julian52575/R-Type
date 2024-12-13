#pragma once

#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include "../../Entities/Entity/Entity.hpp"

namespace System {
    class Collision {
        public:
            Collision();
            ~Collision();
            void update(sparse_array<Position>& positions, sparse_array<Hitbox>& hitboxes, std::function<bool(Entity, Entity)> onCollision);
    };
}

