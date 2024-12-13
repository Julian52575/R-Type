#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include <functional>

namespace System {
    class Script {
        public:
            Script();
            ~Script();
            void updateMovement(sparse_array<Scripting>& scripting,sparse_array<Velocity>& velo,sparse_array<Position>& pos);
            void updateAttack(sparse_array<Scripting>& scripting,sparse_array<Attack>& attack,sparse_array<Position>& pos, float deltaTime, std::function<void(float, float)> spawnProjectile);
    };
};