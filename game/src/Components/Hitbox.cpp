//
#include "Hitbox.hpp"
#include "src/Components/Relationship.hpp"
#include <rengine/src/SparseArray.hpp>

namespace RType {
    namespace Components {

        Hitbox::Hitbox(const RType::Config::EntityConfigHitbox& hitbox)
            : _hitbox(hitbox)
        {
        }

        const Config::EntityConfigHitbox& Hitbox::getSpecs(void) const noexcept
        {
            return this->_hitbox;
        }

        void Hitbox::componentFunction(Rengine::ECS& ecs, RType::Components::Hitbox& hitbox, Rengine::Entity& entity)
        {
            Rengine::SparseArray<RType::Components::Hitbox>& hitboxs = ecs.getComponents<RType::Components::Hitbox>();
            Rengine::SparseArray<RType::Components::Position>& positions = ecs.getComponents<RType::Components::Position>();
            Rengine::SparseArray<RType::Components::Relationship>& relationships = ecs.getComponents<RType::Components::Relationship>();
            std::optional<std::reference_wrapper<Position>> posWrapper = entity.getComponentNoExcept<Position>();

            // Entity has no position
            if (posWrapper == std::nullopt) {
                return;
            }
            // This entity data
            uint16_t ownHitboxStartY = posWrapper->get().getVector2D().y + hitbox.getSpecs().offsetFromSpriteOrigin.y;
            uint16_t ownHitboxEndY = ownHitboxStartY + hitbox.getSpecs().size.y;
            uint16_t ownHitboxStartX = posWrapper->get().getVector2D().x + hitbox.getSpecs().offsetFromSpriteOrigin.x;
            uint16_t ownHitboxEndX = ownHitboxStartX + hitbox.getSpecs().size.x;

            // Parse hitboxs and positions from the current entity to avoid double checks.
            // Assumes hitboxs and positions have the default size defined in the ecs.
            for (Rengine::ECS::size_type index = Rengine::ECS::size_type(entity) + 1; index < ecs.getEntityLimit(); index++) {
                // Check if entity at index has both components
                if (hitboxs[index].has_value() == false || positions[index].has_value() == false) {
                    continue;
                }
                // Check parenty: child cannot damage parents
                if (relationships[index].has_value() == true && relationships[index]->isParented(uint64_t(entity))) {
                    continue;
                }
                // Current entity data
                uint16_t hitboxStartY = positions[index]->getVector2D().y + hitboxs[index]->getSpecs().offsetFromSpriteOrigin.y;
                uint16_t hitboxEndY = hitboxStartY + hitboxs[index]->getSpecs().size.y;
                uint16_t hitboxStartX = positions[index]->getVector2D().x + hitboxs[index]->getSpecs().offsetFromSpriteOrigin.x;
                uint16_t hitboxEndX = hitboxStartX + hitboxs[index]->getSpecs().size.x;

                // Comparison from https://stackoverflow.com/questions/65924292/most-efficient-way-to-compare-all-collision-boxes
                if (!((ownHitboxStartX <= hitboxEndX && ownHitboxEndX >= hitboxStartX) &&
                (ownHitboxStartY <= hitboxEndY && ownHitboxEndY >= hitboxStartY))) {
                    continue;
                }
                std::cout << "Colision between " << int(entity) << " and " << index << std::endl;
                #warning Debug print
            } // for index
        }
    }  // namespace Components
}  // namespace RType
