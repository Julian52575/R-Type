#include "Music.hpp"

Music::Music(const std::string& filename) {
    music.openFromFile(filename);
    this->is_playing = false;
}

Music::~Music() {}

void Music::play() {
    if (is_playing) {
        this->updateStatus();
        return;
    }
    this->is_playing = true;
    music.play();
}

void Music::pause() {
    music.pause();
}

void Music::stop() {
    music.stop();
    this->is_playing = false;
}

void Music::setVolume(float volume) {
    music.setVolume(volume);
}

void Music::setLoop(bool loop) {
    music.setLoop(loop);
}

void Music::updateStatus() {
    if (music.getStatus() == sf::Music::Stopped) {
        this->is_playing = false;
    }
}