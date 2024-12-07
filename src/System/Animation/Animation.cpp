#include "./Animation.hpp"

System::Animation::Animation(){}

System::Animation::~Animation(){}

void System::Animation::update(sparse_array<Component::Animation>& animations, sparse_array<Sprite>& sprites, float deltaTime){
    for (size_t i = 0; i < animations.size() && i < sprites.size(); i++) {
        if (animations[i].has_value() && sprites[i].has_value() && !sprites[i].value().isdefaultTexture){
            animations[i].value().update(deltaTime);
            sprites[i].value().setTextureRect(animations[i].value().getFrame());
        }
    }
}

