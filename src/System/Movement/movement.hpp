#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

namespace System {
    class movement {
        public:
            movement();
            ~movement();
            void update(sparse_array<Position>& positions, sparse_array<Velocity>& velocitiess, float deltaTime);
    };
}

