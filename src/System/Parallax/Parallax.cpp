#include "src/System/Parallax/Parallax.hpp"

System::Parallax::Parallax() {}

System::Parallax::~Parallax() {}

void System::Parallax::update(sparse_array<Position>& positions, sparse_array<Sprite>& sprites, sparse_array<Component::Parallax>& parallaxes, float deltaTime) {
    for (size_t i = 0; i < positions.size() && i < sprites.size() && i < parallaxes.size(); i++) {
        if (positions[i].has_value() && sprites[i].has_value() && parallaxes[i].has_value()) {
            positions[i].value().x += parallaxes[i].value().speed * deltaTime;
            float width = sprites[i].value().getGlobalBounds().width;
            if (positions[i].value().x + width < 0) {
                positions[i].value().x += width;
            }
        }
    }
}

