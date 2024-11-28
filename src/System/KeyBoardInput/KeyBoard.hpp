#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include <cmath>

namespace System {
    class KeyBoardInput {
        public:
            KeyBoardInput();
            ~KeyBoardInput();
            void update(sparse_array<Controllable>& keyBoards, sparse_array<Velocity>& velocities);
    };
}