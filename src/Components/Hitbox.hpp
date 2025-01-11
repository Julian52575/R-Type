#pragma once
#include "src/Config/EntityConfig.hpp"

namespace RType {

    namespace Components {

    class Hitbox {
        private:
        public:
            int off_x;
            int off_y;
            int width;
            int height;
            Hitbox(const RType::Config::EntityConfigHitbox hitbox){
                this->off_x = hitbox.offsetFromSpriteOrigin.x;
                this->off_y = hitbox.offsetFromSpriteOrigin.y;
                this->width = hitbox.size.x;
                this->height = hitbox.size.y;
            }
            ~Hitbox(){}
            static void componentFunction(Rengine::ECS& ecs, RType::Components::Hitbox& hitbox, Rengine::Entity& entity){
                Rengine::SparseArray<RType::Components::Hitbox>& hitboxs = ecs.getComponents<RType::Components::Hitbox>();
                Rengine::SparseArray<RType::Components::Position>& positions = ecs.getComponents<RType::Components::Position>();

                std::optional<std::reference_wrapper<Position>> pos =  entity.getComponentNoExcept<Position>();
                if (pos == std::nullopt)
                    return;

                for (int i = 0; i < hitboxs.size() && i < positions.size(); i++){
                    if (hitboxs[i].has_value() == false || positions[i].has_value() == false)
                        continue;
                    RType::Components::Hitbox& otherHitbox = hitboxs[i].value();
                    RType::Components::Position& otherPosition = positions[i].value();
                    
                    if (i != entity &&
                        pos->get().getVector2D().x + hitbox.off_x <= otherPosition.getVector2D().x + otherHitbox.off_x + otherHitbox.width &&
                        pos->get().getVector2D().x + hitbox.off_x + hitbox.width >= otherPosition.getVector2D().x + otherHitbox.off_x &&
                        pos->get().getVector2D().y + hitbox.off_y <= otherPosition.getVector2D().y + otherHitbox.off_y + otherHitbox.height &&
                        pos->get().getVector2D().y + hitbox.off_y + hitbox.height >= otherPosition.getVector2D().y + otherHitbox.off_y){
                            std::cout << "Collision" << std::endl;
                    }
                }
            }
        };
    }
}
    
