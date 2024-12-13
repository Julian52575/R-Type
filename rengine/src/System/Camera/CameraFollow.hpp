#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include <SFML/Graphics.hpp>

namespace System {
    class CameraFollow{
        public:
            CameraFollow();
            ~CameraFollow();
            void update(sparse_array<Position>& positions, sparse_array<Camera>& cameras,sf::RenderWindow& window);
    };
}  // namespace System
