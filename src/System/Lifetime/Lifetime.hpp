#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

namespace System {
    class Lifetime {
        public:
            Lifetime();
            ~Lifetime();
            void update(sparse_array<Component::LifeTime>& lifetimes,float deltaTime);
    };
}
