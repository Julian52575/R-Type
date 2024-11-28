#pragma once
#include <SFML/Graphics.hpp>

class Sprite : public sf::Sprite {
    public:
        Sprite(const std::string& filename,std::pair<float,float> scale = std::make_pair(1.f,1.f),std::pair<float,float> origin = std::make_pair(0.f,0.f));
        ~Sprite();

    private:
        sf::Texture texture;
        void MakeTexture();
        
};