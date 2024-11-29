#include "Sprite.hpp"

Sprite::Sprite(const std::string& filename,std::pair<float,float> scale,std::pair<float,float> origin) {
    this->texture = std::make_shared<sf::Texture>();
    if(!(*texture).loadFromFile(filename))
        this->MakeTexture();
    else
        setTexture(*texture);    
    setOrigin(origin.first,origin.second);
    setScale(scale.first,scale.second);
}

Sprite::~Sprite() {}

sf::Texture &Sprite::getTexture() {
    return *texture;
}


void Sprite::MakeTexture(){
    if (!(*texture).create(50, 50)) {
        throw std::runtime_error("Could not create texture");
    }
    sf::Image image;
    image.create(50, 50, sf::Color::Black);
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25; j++) {
            image.setPixel(i, j, sf::Color::Magenta);
        }
    }
    for (int i = 25; i < 50; i++) {
        for (int j = 25; j < 50; j++) {
            image.setPixel(i, j, sf::Color::Magenta);
        }
    }
    (*texture).update(image);
    setTexture(*texture);
}