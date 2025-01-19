//
#ifndef SRC_COMPONENTS_HITBOX_HPP_
#define SRC_COMPONENTS_HITBOX_HPP_

#include "src/Config/EntityConfig.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Relationship.hpp"
#include <rengine/src/ECS.hpp>
#include <rengine/src/SparseArray.hpp>

namespace RType {
    namespace Components {

        class Hitbox {
            public:
                Hitbox(const RType::Config::EntityConfigHitbox& hitbox);
                ~Hitbox(void) = default;
                const Config::EntityConfigHitbox& getSpecs(void) const noexcept;

                static void componentFunction(Rengine::ECS& ecs, RType::Components::Hitbox& hitbox, Rengine::Entity& entity);

            private:
                Config::EntityConfigHitbox _hitbox;
                float _lastCheckSeconds = 0;
        };  // class Hitbox
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_HITBOX_HPP_
