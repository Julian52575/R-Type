#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

namespace bsf {
    class Texture : public sf::Texture {
        public:
            Texture();
            Texture(const std::string& filename);
            ~Texture();
            bool loadFromFile(const std::string& filename,const sf::IntRect& area = sf::IntRect());

        private:
            void makeDefaultTexture();
    };
}