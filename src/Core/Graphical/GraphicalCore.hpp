#pragma once

#include "../../Registry.hpp"
#include "../../Component/Liste.hpp"
#include "../../Entities/EntityManager/Manager.hpp"
#include "../../System/Liste.hpp"
#include "../../Entities/Maker/EntityMaker.hpp"

#include <memory>
#include <string>

class GraphicalCore {
    public:
        GraphicalCore();
        ~GraphicalCore();
        void destroy_entity(Entity e);
        void run();
        Entity MakeEntity(const std::string path);


    private:
        EntityManager em;
        std::unique_ptr<EntityMaker> maker;
        
        System::movement movement;
        System::KeyBoardInput keyBoardInput;
        System::Collision collision;
        System::Parallax parallaxSystem;
        System::Audio audio;
        System::Animation animations;
        System::Render render;
        System::CameraFollow cameraFollow;
        System::Lifetime lifetimeSystem;
};

