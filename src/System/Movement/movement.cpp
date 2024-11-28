#include "movement.hpp"

System::movement::movement(){}

System::movement::~movement(){}

void System::movement::update(sparse_array<Position>& positions, sparse_array<Velocity>& velocities, float deltaTime){
    for (size_t i = 0; i < positions.size() && i < velocities.size(); i++) {
        if (positions[i].has_value() && velocities[i].has_value()) {
            positions[i].value().x += (velocities[i].value().x * deltaTime);
            positions[i].value().y += (velocities[i].value().y * deltaTime);
        }
    }
}