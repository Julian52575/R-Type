#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

namespace System {
    class Animation {
        public:
            Animation();
            ~Animation();
            void update(sparse_array<Component::Animation>& animations, sparse_array<Sprite>& sprites, float deltaTime);
    };
}

