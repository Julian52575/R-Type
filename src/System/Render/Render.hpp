#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"
#include <SFML/Graphics.hpp>

namespace System {
    class Render {
        public:
            Render();
            ~Render();
            bool isOpen();
            sf::RenderWindow& getWindow();
            void processEvents();
            void update(sparse_array<Position>& positions, sparse_array<Sprite>& sprites,sparse_array<Component::Parallax>& parallaxes);

        private:
            sf::RenderWindow window;
    };
}