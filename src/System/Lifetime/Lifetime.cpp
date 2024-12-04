#include "Lifetime.hpp"

System::Lifetime::Lifetime() {}

System::Lifetime::~Lifetime() {}

void System::Lifetime::update(sparse_array<Component::LifeTime>& lifetimes,float deltaTime) {
    for (size_t i = 0; i < lifetimes.size(); i++) {
        if (lifetimes[i].has_value()) {
            lifetimes[i].value().update(deltaTime);
            if (lifetimes[i].value().isDead()) {
                // lifetimes.erase_at(i);
                // gérer la supression de l'entité
            }
        }
    }
}