#include "./Collision.hpp"

System::Collision::Collision() {}

System::Collision::~Collision() {}

void System::Collision::update(sparse_array<Position>& positions, sparse_array<Hitbox>& hitboxes) {
    for (uint32_t i = 0; i < positions.size() && i < hitboxes.size(); i++) {
        if (positions[i].has_value() && hitboxes[i].has_value()) {
            for (uint32_t j = i + 1; j < positions.size() && j < hitboxes.size(); j++) {
                if (positions[j].has_value() && hitboxes[j].has_value()) {
                    float pos1x = positions[i].value().x + hitboxes[i].value().getOffset().x;
                    float pos1y = positions[i].value().y + hitboxes[i].value().getOffset().y;
                    float pos2x = positions[j].value().x + hitboxes[j].value().getOffset().x;
                    float pos2y = positions[j].value().y + hitboxes[j].value().getOffset().y;

                    if (pos1x < pos2x + hitboxes[j].value().getSize().x &&
                        pos1x + hitboxes[i].value().getSize().x > pos2x &&
                        pos1y < pos2y + hitboxes[j].value().getSize().y &&
                        pos1y + hitboxes[i].value().getSize().y > pos2y) {
                        std::cout << "Collision detected between " << i << " and " << j << std::endl;
                    }
                }
            }
        }
    }
}

