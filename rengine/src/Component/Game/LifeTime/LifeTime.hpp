#pragma once

namespace Component {
  
    class LifeTime{
        public:
            explicit LifeTime(float time);
            ~LifeTime();
            void update(float deltaTime);
            bool isDead();
        private:
            float time;
            float current_time;
    };
}

