#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include <cmath>
#include <functional>

namespace System {
    class KeyBoardInput {
        public:
            KeyBoardInput();
            ~KeyBoardInput();
            void update(sparse_array<Controllable>& keyBoards, sparse_array<Velocity>& velocities);
            void shoot(sparse_array<Controllable>& keyBoards, sparse_array<Position>& positions, sparse_array<Attack>& attacks,float deltatime, std::function<void(float, float)> makePlayerAttack);
    };
}
