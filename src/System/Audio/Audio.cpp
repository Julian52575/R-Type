#include "src/System/Audio/Audio.hpp"

System::Audio::Audio() {}

System::Audio::~Audio() {}

void System::Audio::update(sparse_array<Music>& musics, sparse_array<Sound>& sounds) {
    for (size_t i = 0; i < musics.size(); i++) {
        if (musics[i].has_value()) {
            musics[i].value().play();
        }
    }
    for (size_t i = 0; i < sounds.size(); i++) {
        if (sounds[i].has_value()) {
            sounds[i].value().play();
        }
    }
}

