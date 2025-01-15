//
#include "Hitbox.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/Configuration.hpp"
#include "src/Components/Life.hpp"

#include <rengine/src/Graphics/GraphicManager.hpp>
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
            Rengine::SparseArray<RType::Components::Life>& lifes = ecs.getComponents<RType::Components::Life>();

            std::optional<std::reference_wrapper<Position>> posWrapper = entity.getComponentNoExcept<Position>();

            // Entity has no position
            if (posWrapper == std::nullopt) {
                return;
            }
            // This entity data
            float ownHitboxStartY = posWrapper->get().getVector2D().y + hitbox.getSpecs().offsetFromSpriteOrigin.y;
            float ownHitboxEndY = ownHitboxStartY + hitbox.getSpecs().size.y;
            float ownHitboxStartX = posWrapper->get().getVector2D().x + hitbox.getSpecs().offsetFromSpriteOrigin.x;
            float ownHitboxEndX = ownHitboxStartX + hitbox.getSpecs().size.x;

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
                float hitboxStartY = positions[index]->getVector2D().y + hitboxs[index]->getSpecs().offsetFromSpriteOrigin.y;
                float hitboxEndY = hitboxStartY + hitboxs[index]->getSpecs().size.y;
                float hitboxStartX = positions[index]->getVector2D().x + hitboxs[index]->getSpecs().offsetFromSpriteOrigin.x;
                float hitboxEndX = hitboxStartX + hitboxs[index]->getSpecs().size.x;

                // Comparison from https://stackoverflow.com/questions/65924292/most-efficient-way-to-compare-all-collision-boxes
                if (!(ownHitboxStartX <= hitboxEndX && ownHitboxEndX >= hitboxStartX &&
                    ownHitboxStartY <= hitboxEndY && ownHitboxEndY >= hitboxStartY)) {
                    continue;
                }
                float currentTime = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getElapsedTimeSeconds();
                float timeCooldown = 1.5;

                if (currentTime - hitbox._lastCheckSeconds < timeCooldown) {
                    return;
                }
                hitbox._lastCheckSeconds = currentTime;
                std::cout << "Colision between " << Rengine::ECS::size_type(entity) << " and " << index << std::endl;

                //check si l'entité à une vie et si c'est le cas elle take damage
                if (lifes[index].has_value() == true) {
                    lifes[index]->takeDamage(1);// penser à mettre les degats de entity
                }

                // ecs.removeEntity(ecs.getEntity(index));
                return;  // Only one collision per frame
                
                #warning Debug print
            } // for index
        }
    }  // namespace Components
}  // namespace RType
