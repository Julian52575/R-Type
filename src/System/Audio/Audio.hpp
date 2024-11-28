#pragma once
#include <SFML/Audio.hpp>
#include "../../Sparse_Array.hpp"
#include "../../Component/Liste.hpp"

namespace System {
    class Audio {
        public:
            Audio();
            ~Audio();
            void update(sparse_array<Music>& musics, sparse_array<Sound>& sounds);
    };
};