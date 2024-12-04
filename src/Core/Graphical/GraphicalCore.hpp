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
        void MakePlayer();
        void MakeBackground();
        void MakeParallaxBackground(const std::string& texturePath, float speed);
        void MakeMessage();

    private:
        registry reg;
        EntityManager em;
        std::unique_ptr<EntityMaker> maker;
        sparse_array<Acceleration> acc;
        sparse_array<Position> pos;
        sparse_array<Rotation> rot;
        sparse_array<Scale> scale;
        sparse_array<Velocity> velo;
        sparse_array<Component::Animation> animation;
        sparse_array<Music> music;
        sparse_array<Component::Parallax> parallax;
        sparse_array<Shader> shader;
        sparse_array<Sound> sound;
        sparse_array<Sprite> sprite;
        sparse_array<Text> text;
        sparse_array<Attack> attack;
        sparse_array<Group> group;
        sparse_array<Health> health;
        sparse_array<Hitbox> hitbox;
        sparse_array<Component::LifeTime> lifetime;
        sparse_array<Scripting> scripting;
        sparse_array<Clickable> clickable;
        sparse_array<Controllable> controllable;
        System::movement movement;
        System::KeyBoardInput keyBoardInput;
        System::Collision collision;
        System::Parallax parallaxSystem;
        System::Audio audio;
        System::Animation animations;
        System::Render render;
        System::Lifetime lifetimeSystem;
};

