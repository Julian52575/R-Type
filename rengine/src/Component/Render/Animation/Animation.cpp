#include "./Animation.hpp"

Component::Animation::Animation(sf::IntRect rectSize, int nbrFrames, float speed){
    this->rectSize = rectSize;
    this->currentFrame = 0;
    this->time = 0.0f;
    this->nbrFrames = nbrFrames;
    this->speed = speed;
}

Component::Animation::Animation::~Animation(){}

void Component::Animation::update(float dt){
    this->time += dt;
    if (this->time >= this->speed) {
        this->time = 0.0f;
        this->currentFrame = (this->currentFrame + 1) % this->nbrFrames;
    }
}

void Component::Animation::setFrame(int frame){
    this->currentFrame = frame;
}

sf::IntRect Component::Animation::getFrame(){
    return sf::IntRect(rectSize.left + rectSize.width * currentFrame, rectSize.top, rectSize.width, rectSize.height);
}

