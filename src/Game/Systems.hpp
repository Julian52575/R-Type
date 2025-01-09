#pragma once

#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>

#include "SceneManager.hpp"
#include "Scenes.hpp"


class Systems{
    public:
        Systems();
        ~Systems();
        void InputSystem(Rengine::Entity& entity,float deltatime,Rengine::ECS& ecs,RType::SceneManager<Scene>& sceneManager,Rengine::Graphics::GraphicManager& graphicManager);
        void MovementSystem(Rengine::Entity& entity,float deltatime);
        void renderSpriteSystem(Rengine::Entity& entity);      
};