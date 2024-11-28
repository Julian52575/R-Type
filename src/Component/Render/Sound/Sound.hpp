#pragma once
#include <SFML/Audio.hpp>

class Sound {
        public:
            Sound(const std::string& filename);
            ~Sound();
            void play();
            void pause();
            void stop();
            void setVolume(float volume);
            void setLoop(bool loop);
            void updateStatus();
        private:
            sf::SoundBuffer buffer;
            sf::Sound sound;
            bool is_playing;
            bool can_play;
    };