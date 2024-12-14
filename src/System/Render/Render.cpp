#include "./Render.hpp"

System::Render::Render() {
    window.create(sf::VideoMode(800, 600), "ECS");
    this->shader.loadFromFile("assets/shaders/colorblind_default.frag", sf::Shader::Fragment);
    this->shaderIndex = 0;
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
        if (event.type == sf::Event::KeyPressed && window.hasFocus()) {
            if (event.key.code == sf::Keyboard::D) {
                shaderIndex = (shaderIndex + 1) % shaders.size();
                shader.loadFromFile(shaders[shaderIndex], sf::Shader::Fragment);
            }
        }
    }
}

void System::Render::update(sparse_array<Position>& positions, sparse_array<Sprite>& sprites, sparse_array<Component::Parallax>& parallaxes,sparse_array<Text>& texts, sparse_array<Hitbox>& hitboxes) {
    window.clear();

    std::vector<uint32_t> parallaxIndices;
    for (uint32_t i = 0; i < positions.size() && i < parallaxes.size() && i < sprites.size(); i++) {
        if (positions[i].has_value() && parallaxes[i].has_value() && sprites[i].has_value()) {
            parallaxIndices.push_back(i);
            float width = sprites[i].value().getGlobalBounds().width;
            float x = positions[i].value().x;
            while (x < window.getSize().x) {
                sprites[i].value().setPosition(x, positions[i].value().y);
                window.draw(sprites[i].value(), &shader);
                x += width;
            }
        }
    }

    for (uint32_t i = 0; i < positions.size() && i < sprites.size(); i++) {
        if (positions[i].has_value() && sprites[i].has_value() && std::find(parallaxIndices.begin(), parallaxIndices.end(), i) == parallaxIndices.end()) {
            sprites[i].value().setPosition(positions[i].value().x, positions[i].value().y);
            window.draw(sprites[i].value(), &shader);
        }
    }

    for (uint32_t i = 0; i < positions.size() && i < hitboxes.size(); i++) {
        if (positions[i].has_value() && hitboxes[i].has_value()) {
            sf::RectangleShape hitbox;
            hitbox.setSize(hitboxes[i].value().getSize());
            hitbox.setPosition(positions[i].value().x + hitboxes[i].value().getOffset().x, positions[i].value().y + hitboxes[i].value().getOffset().y);
            hitbox.setFillColor(sf::Color::Transparent);
            hitbox.setOutlineColor(sf::Color::Red);
            hitbox.setOutlineThickness(1);
            window.draw(hitbox);
        }
    }

    for (uint32_t i = 0; i < texts.size() && i < positions.size(); i++) {
        if (texts[i].has_value() && positions[i].has_value()) {
            texts[i].value().setPosition(positions[i].value().x, positions[i].value().y);
            window.draw(texts[i].value(), &shader);
        }
    }

    window.display();
}

