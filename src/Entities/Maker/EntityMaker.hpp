#pragma once
#include "../Entity/Entity.hpp"
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

#include <SFML/Graphics.hpp>
#include <utility>
#include <string>
#include <unordered_map>
#include <memory>

class EntityMaker {
    public:
        EntityMaker();
        ~EntityMaker();
        void setEntity(Entity e);
        void setSprite(const std::string& texturePath, std::pair<float, float> scale, std::pair<float, float> origin, sparse_array<Sprite>& spriteList);
        void setPosition(float x, float y, sparse_array<Position>& positionList);
        void setVelocity(float x, float y, sparse_array<Velocity>& velocityList);
        void setControllable(sparse_array<Controllable>& controllableList);
        void setAnimations(const sf::IntRect& rect, int frames, float duration, sparse_array<Component::Animation>& animationList);
        void setParallax(float speed, sparse_array<Component::Parallax>& parallaxList);
        void setText(const std::string& text, const std::string& fontPath, int size, const sf::Color& color, sparse_array<Text>& textList);

    private:
        std::shared_ptr<Entity> e;
};

