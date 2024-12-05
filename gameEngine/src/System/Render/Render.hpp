#pragma once
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace System {
    class Render {
        public:
            Render();
            ~Render();
            bool isOpen();
            sf::RenderWindow& getWindow();
            void processEvents();
            void update(sparse_array<Position>& positions, sparse_array<Sprite>& sprites, sparse_array<Component::Parallax>& parallaxes, sparse_array<Text>& texts,sparse_array<Hitbox>& hitboxes);

        private:
            sf::RenderWindow window;
            sf::Shader shader;
            std::vector<std::string> shaders = {"shaders/colorblind_default.frag", "shaders/colorblind_deuteranopia.frag", "shaders/colorblind_protanopia.frag", "shaders/colorblind_tritanopia.frag"};
            int shaderIndex;
    };
}  // namespace System
