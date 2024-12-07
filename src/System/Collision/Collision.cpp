#include "./Collision.hpp"

System::Collision::Collision() {}

System::Collision::~Collision() {}

void System::Collision::update(sparse_array<Position>& positions, sparse_array<Hitbox>& hitboxes) {
    for (uint32_t i = 0; i < positions.size() && i < hitboxes.size(); i++) {
        if (positions[i].has_value() && hitboxes[i].has_value()) {
            for (uint32_t j = i + 1; j < positions.size() && j < hitboxes.size(); j++) {
                if (positions[j].has_value() && hitboxes[j].has_value()) {
                    // check la collision ici
                }
            }
        }
    }
}

