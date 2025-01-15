#pragma once

#include "src/Config/EntityConfig.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Relationship.hpp"
#include <rengine/src/ECS.hpp>
#include <rengine/src/SparseArray.hpp>


namespace RType {
    namespace Components {

        class HealthViewer {
            public:
                HealthViewer(void) = default;
                ~HealthViewer(void) = default;
                const Config::EntityConfigHitbox& getSpecs(void) const noexcept;

                static void componentFunction(Rengine::ECS& ecs, RType::Components::HealthViewer& health_view, Rengine::Entity& entity);

            private:
        };  // class HealthViewer
    }  // namespace Components
}  // namespace RType