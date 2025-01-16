//
#include "Hitbox.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/Configuration.hpp"
#include "src/Components/Life.hpp"

#include <functional>
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
            Rengine::SparseArray<RType::Components::Configuration>& configurations = ecs.getComponents<RType::Components::Configuration>();

            std::optional<std::reference_wrapper<Position>> posWrapper = entity.getComponentNoExcept<Position>();
            std::optional<std::reference_wrapper<Life>> lifeWrapper = entity.getComponentNoExcept<Life>();
            std::optional<std::reference_wrapper<Configuration>> configWrapper = entity.getComponentNoExcept<Configuration>();
            std::optional<std::reference_wrapper<Relationship>> relationshipWrapper = entity.getComponentNoExcept<Relationship>();

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
                // Check entity relationship
                if (relationships[index].has_value() == true && relationships[index]->isRelated(uint64_t(entity))) {
                    continue;
                }
                // Check host relationship
                if (relationshipWrapper.has_value() == true && relationshipWrapper->get().isRelated(index)) {
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
                float timeCooldown = 0.3;

                // if (currentTime - hitbox._lastCheckSeconds < timeCooldown) {
                //     return;
                // }
                hitbox._lastCheckSeconds = currentTime;
                //check si l'entité à une vie et si c'est le cas elle take damage
                if (lifes[index].has_value() == true) {
                    if (configWrapper.has_value() == true) {
                        lifes[index]->takeDamage(configWrapper->get().getConfig().getStats().attack);
                    }
                    else {
                        lifes[index]->takeDamage(1);
                    }
                }
                if (lifeWrapper.has_value() == true) {
                    if (configurations[index].has_value() == true) {
                        lifeWrapper->get().takeDamage(configurations[index]->getConfig().getStats().attack);
                    }
                    else {
                        lifeWrapper->get().takeDamage(1);
                    }
                }

            } // for index
        }
    }  // namespace Components
}  // namespace RType
