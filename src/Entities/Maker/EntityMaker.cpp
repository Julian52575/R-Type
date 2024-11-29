#include "EntityMaker.hpp"

EntityMaker::EntityMaker() {}

EntityMaker::~EntityMaker(){}

void EntityMaker::setEntity(Entity e){
    this->e = std::make_shared<Entity>(e);
}

void EntityMaker::setSprite(const std::string& texturePath, std::pair<float,float> scale, std::pair<float,float> origin, sparse_array<Sprite>& spriteList){
    spriteList.emplace_at(*e, texturePath, scale, origin);
}

void EntityMaker::setPosition(float x, float y, sparse_array<Position>& positionList){
    positionList.emplace_at(*e, x, y);
}

void EntityMaker::setVelocity(float x, float y, sparse_array<Velocity>& velocityList){
    velocityList.emplace_at(*e, x, y);
}

void EntityMaker::setControllable(sparse_array<Controllable>& controllableList){
    controllableList.emplace_at(*e);
}

void EntityMaker::setAnimations(const sf::IntRect& rect, int frames, float duration, sparse_array<Component::Animation>& animationList){
    animationList.emplace_at(*e, rect, frames, duration);
}

void EntityMaker::setParallax(float speed, sparse_array<Component::Parallax>& parallaxList){
    parallaxList.emplace_at(*e, speed);
}