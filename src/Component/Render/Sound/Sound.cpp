#include "src/Component/Render/Sound/Sound.hpp"
#include <string>

Sound::Sound(const std::string& filename) {
    buffer.loadFromFile(filename);
    sound.setBuffer(buffer);
    this->is_playing = false;
    this->can_play = true;
}

Sound::~Sound() {}

void Sound::play() {
    if (is_playing) {
        this->updateStatus();
        return;
    }
    if (!can_play) {
        return;
    }
    this->is_playing = true;
    sound.play();
}

void Sound::pause() {
    sound.pause();
}

void Sound::stop() {
    sound.stop();
    this->is_playing = false;
}

void Sound::setVolume(float volume) {
    sound.setVolume(volume);
}

void Sound::setLoop(bool loop) {
    sound.setLoop(loop);
}

void Sound::updateStatus() {
    if (sound.getStatus() == sf::Sound::Stopped) {
        this->is_playing = false;
        this->can_play = false;
    }
}
