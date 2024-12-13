#pragma once

#include "../Registry.hpp"
#include "../Component/Liste.hpp"
#include "../Entities/EntityManager/Manager.hpp"
#include "../System/Liste.hpp"
#include "../Entities/Maker/EntityMaker.hpp"

#include <memory>
#include <string>

namespace Rengine {

    class Rengine {
        public:
            Rengine();
            ~Rengine();
            void destroyEntity(Entity e);
            Entity& makeEntity(const std::string& path);

        public:
            System::movement& getMovement(void);
            System::KeyBoardInput& getKeyBoardInput(void);
            System::Collision& getCollision(void);
            System::Parallax& getParallax(void);
            System::Audio& getAudio(void);
            System::Animation& getAnimation(void);
            System::Render& getRender(void);
            System::CameraFollow& getCameraFollow(void);
            System::Lifetime& getLifetime(void);
            EntityManager& getEntityManager(void);
            EntityMaker& getEntityMaker(void);

        private:
            EntityManager _entityManager;
            EntityMaker _entityMaker;  // uniqueptr
            System::movement _movement;
            System::KeyBoardInput _keyBoardInput;
            System::Collision _collision;
            System::Parallax _parallaxSystem;
            System::Audio _audio;
            System::Animation _animations;
            System::Render _render;
            System::CameraFollow _cameraFollow;
            System::Lifetime _lifetimeSystem;
    };  // class Core

}  // namespace Rengine

