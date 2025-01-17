//
#ifndef SRC_COMPONENTS_ALRERT_HPP_
#define SRC_COMPONENTS_ALERT_HPP_

#include "src/Config/EntityConfig.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/Hitbox.hpp"
#include <rengine/src/ECS.hpp>
#include <rengine/src/SparseArray.hpp>

namespace RType {
    namespace Components {

        class Alert {
            public:
                Alert(const RType::Config::EntityConfigHitbox& hitbox);
                ~Alert(void) = default;
                const Config::EntityConfigHitbox& getSpecs(void) const noexcept;

                static void componentFunction(Rengine::ECS& ecs, RType::Components::Alert& hitbox, Rengine::Entity& entity);

            private:
                Config::EntityConfigHitbox _hitbox;
        };  // class Alert
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_ALERT_HPP_
