#include "./Lifetime.hpp"

System::Lifetime::Lifetime() {}

System::Lifetime::~Lifetime() {}

std::vector<Entity> System::Lifetime::update(sparse_array<Component::LifeTime>& lifetimes,float deltaTime) {
    std::vector<Entity> entities;
    for (size_t i = 0; i < lifetimes.size(); i++) {
        if (lifetimes[i].has_value()) {
            lifetimes[i].value().update(deltaTime);
            if (lifetimes[i].value().isDead()) {
                entities.push_back(Entity(i));
            }
        }
    }
    return entities;
}
