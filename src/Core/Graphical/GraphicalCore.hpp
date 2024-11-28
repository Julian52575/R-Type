#pragma once

#include "../../Registry.hpp"
#include "../../Component/Liste.hpp"
#include "../../Entities/EntityManager/Manager.hpp"
#include "../../System/Liste.hpp"

class GraphicalCore {
    public:
        GraphicalCore();
        ~GraphicalCore();
        void destroy_entity(Entity e);
        void run();

        void MakePlayer();

    private:
        registry reg;
        EntityManager em;

        sparse_array<Position> pos;
        sparse_array<Velocity> velo;
        sparse_array<Acceleration> acc;
        sparse_array<Rotation> rot;
        sparse_array<Scale> scale;

        sparse_array<Sprite> sprite;
        sparse_array<Component::Animation> animation;
        // sparse_array<Sound> sounds;
        // sparse_array<Music> musics;

        sparse_array<Controllable> controllable;
        


        System::movement movement;
        System::KeyBoardInput keyBoardInput;
        System::Audio audio;
        System::Animation animations;
        System::Render render;
        
};