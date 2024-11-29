#include "Render.hpp"

System::Render::Render() {
    window.create(sf::VideoMode(800, 600), "ECS");
}

System::Render::~Render() {}

bool System::Render::isOpen() {
    return window.isOpen();
}

sf::RenderWindow& System::Render::getWindow() {
    return window;
}

void System::Render::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void System::Render::update(sparse_array<Position>& positions, sparse_array<Sprite>& sprites,sparse_array<Component::Parallax>& parallaxes) {
    window.clear();
    for (size_t i = 0; i < positions.size() && i < parallaxes.size() && i < sprites.size(); i++) {
        if (positions[i].has_value() && parallaxes[i].has_value() && sprites[i].has_value()) {
            float width = sprites[i].value().getGlobalBounds().width;
            float x = positions[i].value().x;
            while (x < window.getSize().x) {
                sprites[i].value().setPosition(x, positions[i].value().y);
                window.draw(sprites[i].value());
                x += width;
            }
        }
    }
    
    for (size_t i = 0; i < positions.size() && i < sprites.size(); i++) {
        if (positions[i].has_value() && sprites[i].has_value()) {
            sprites[i].value().setPosition(positions[i].value().x, positions[i].value().y);
            window.draw(sprites[i].value());
        }
    }
    window.display();
}