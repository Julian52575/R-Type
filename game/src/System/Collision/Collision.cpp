#include "src/System/Collision/Collision.hpp"

System::Collision::Collision() {}

System::Collision::~Collision() {}

void System::Collision::update(sparse_array<Position>& positions, sparse_array<Hitbox>& hitboxes) {
    for (size_t i = 0; i < positions.size() && i < hitboxes.size(); i++) {
        if (positions[i].has_value() && hitboxes[i].has_value()) {
            for (size_t j = i + 1; j < positions.size() && j < hitboxes.size(); j++) {
                if (positions[j].has_value() && hitboxes[j].has_value()) {
                    // check la collision ici
                }
            }
        }
    }
}

