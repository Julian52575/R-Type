/*
#include "Alert.hpp"
#include "src/Components/Relationship.hpp"
#include <rengine/src/SparseArray.hpp>

namespace RType {
    namespace Components {

        Alert::Alert(const RType::Config::EntityConfigHitbox& hitbox)
            : _hitbox(hitbox)
        {
        }

        const Config::EntityConfigHitbox& Alert::getSpecs(void) const noexcept
        {
            return this->_hitbox;
        }

        void Alert::componentFunction(Rengine::ECS& ecs, RType::Components::Alert& hitbox, Rengine::Entity& entity)
        {
            std::cout << "Alert component function" << std::endl;
            Rengine::SparseArray<RType::Components::Hitbox>& hitboxs = ecs.getComponents<RType::Components::Hitbox>();
            Rengine::SparseArray<RType::Components::Position>& positions = ecs.getComponents<RType::Components::Position>();
            Rengine::SparseArray<RType::Components::Relationship>& relationships = ecs.getComponents<RType::Components::Relationship>();
            std::optional<std::reference_wrapper<Position>> posWrapper = entity.getComponentNoExcept<Position>();

            // Entity has no position
            if (posWrapper == std::nullopt) {
                return;
            }
            // This entity data
            uint16_t ownPositionY = posWrapper->get().getVector2D().y;
            uint16_t ownPositionX = posWrapper->get().getVector2D().x;

            // Parse hitboxs and positions from the current entity to avoid double checks.
            // Assumes hitboxs and positions have the default size defined in the ecs.
            for (Rengine::ECS::size_type index = Rengine::ECS::size_type(entity) + 1; index < ecs.getEntityLimit(); index++) {
                // Check if entity at index has both components
                if (hitboxs[index].has_value() == false || positions[index].has_value() == false) {
                    continue;
                }
                // Check parenty: child cannot damage parents
                if (relationships[index].has_value() == true && relationships[index]->isRelated(uint64_t(entity))) {
                    continue;
                }
                // Current entity data
                uint16_t positionY = positions[index]->getVector2D().y;
                uint16_t positionX = positions[index]->getVector2D().x;

                // Checks if the entity is in the alert range
                uint16_t alertRange = 200;
                float distance = sqrt(pow(ownPositionX - positionX, 2) + pow(ownPositionY - positionY, 2));
                std::cout << "Distance between " << int(entity) << " and " << index << " is " << distance << std::endl;
                if (distance < alertRange) {
                    continue;
                }
                std::cout << "Alert between " << int(entity) << " and " << index << std::endl;
                #warning Debug print
            } // for index
        }
    }  // namespace Components
}  // namespace RType
*/
