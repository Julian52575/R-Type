#include "Texture.hpp"

bsf::Texture::Texture() {
    makeDefaultTexture();
}

bsf::Texture::Texture(const std::string& filename) {
    loadFromFile(filename);
}

bsf::Texture::~Texture() {
}

bool bsf::Texture::loadFromFile(const std::string& filename, const sf::IntRect& area) {
    if (!sf::Texture::loadFromFile(filename, area)) {
        std::cerr << "Error while loading texture " << filename << ": using default texture" << std::endl;
        makeDefaultTexture();
    }
    return true;
}

void bsf::Texture::makeDefaultTexture() {
    sf::Image image;
    image.create(100, 100, sf::Color::Green);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (i % 10 == 0) {
                image.setPixel(i, j, sf::Color::Black);
            }
        }
    }
    loadFromImage(image);
}