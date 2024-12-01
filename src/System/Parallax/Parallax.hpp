#pragma once
#include <SFML/Graphics.hpp>
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

namespace System {
    class Parallax {
        public:
            Parallax();
            ~Parallax();
            void update(sparse_array<Position>& positions, sparse_array<Sprite>& sprites, sparse_array<Component::Parallax>& parallaxes, float deltaTime);
    };
};