#pragma once
#include "../Entity/Entity.hpp"
#include <SFML/Graphics.hpp>
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

#include <unordered_map>
#include <memory>

class EntityMaker {
    public:
        EntityMaker() {}
        
        ~EntityMaker(){}

        void setEntity(Entity e){
            this->e = std::make_shared<Entity>(e);
        }

        void setSprite(const std::string& texturePath, std::pair<float,float> scale, std::pair<float,float> origin, sparse_array<Sprite>& spriteList){
            spriteList.emplace_at(*e, texturePath, scale, origin);
        }

        void setPosition(float x, float y, sparse_array<Position>& positionList){
            positionList.emplace_at(*e, x, y);
        }

        void setVelocity(float x, float y, sparse_array<Velocity>& velocityList){
            velocityList.emplace_at(*e, x, y);
        }

        void setControllable(sparse_array<Controllable>& controllableList){
            controllableList.emplace_at(*e);
        }

        void setAnimations(const sf::IntRect& rect, int frames, float duration, sparse_array<Component::Animation>& animationList){
            animationList.emplace_at(*e, rect, frames, duration);
        }

        void setParallax(float speed, sparse_array<Component::Parallax>& parallaxList){
            parallaxList.emplace_at(*e, speed);
        }
        
    private:
        std::shared_ptr<Entity> e;
};