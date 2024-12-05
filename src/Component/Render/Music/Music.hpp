#pragma once
#include <string>
#include <SFML/Audio.hpp>
#include <memory>

class Music {
        public:
            explicit Music(const std::string& filename);
            ~Music();
            void play();
            void pause();
            void stop();
            void setVolume(float volume);
            void setLoop(bool loop);
            void updateStatus();
        private:
            std::shared_ptr<sf::Music> music;
            bool is_playing;
};

