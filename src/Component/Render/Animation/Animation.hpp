#pragma once
#include <SFML/Graphics.hpp>

namespace Component{
    class Animation{
        public:
            Animation(sf::IntRect rectSize, int nbrFrames, float speed);
            ~Animation();
            void update(float dt);
            void setFrame(int frame);
            sf::IntRect getFrame();

        private:
            sf::IntRect rectSize;
            int currentFrame;
            float time;
            int nbrFrames;
            float speed;
    };
}