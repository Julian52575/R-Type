#pragma once
#include <SFML/Audio.hpp>

class Music {
        public:
            Music(const std::string& filename);
            ~Music();
            void play();
            void pause();
            void stop();
            void setVolume(float volume);
            void setLoop(bool loop);
            void updateStatus();
        private:
            sf::Music music;
            bool is_playing;
    };