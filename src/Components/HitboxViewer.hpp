#pragma once

#include "src/Config/EntityConfig.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Relationship.hpp"
#include <rengine/src/ECS.hpp>
#include <rengine/src/SparseArray.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>

namespace RType {
    namespace Components {

        class HitboxViewer {
            public:
                HitboxViewer(float width, float height);
                ~HitboxViewer(void);
                const Config::EntityConfigHitbox& getSpecs(void) const noexcept;

                static void componentFunction(Rengine::ECS& ecs, RType::Components::HitboxViewer& hitbox, Rengine::Entity& entity);

            private:
                std::shared_ptr<Rengine::Graphics::ASprite> _hitboxSprite;
        };  // class HitboxViewer
    }  // namespace Components
}  // namespace RType
