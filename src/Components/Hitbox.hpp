#pragma once
#include "src/Config/EntityConfig.hpp"

namespace RType {

    namespace Components {

    class Hitbox {
        private:
            int _x;
            int _y;
            int _width;
            int _height;
        public:
            Hitbox(const RType::Config::EntityConfigHitbox hitbox){
                this->_x = hitbox.offsetFromSpriteOrigin.x;
                this->_y = hitbox.offsetFromSpriteOrigin.y;
                this->_width = hitbox.size.x;
                this->_height = hitbox.size.y;
            }
            ~Hitbox(){}
            static void componentFunction(Rengine::ECS& ecs, RType::Components::Hitbox& hitbox, Rengine::Entity& entity){
                Rengine::SparseArray<RType::Components::Hitbox>& hitboxs = ecs.getComponents<RType::Components::Hitbox>();
            }
        };
    }
}
    
