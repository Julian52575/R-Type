#pragma once

#include "src/Config/EntityConfig.hpp"
#include <rengine/src/ECS.hpp>
#include <rengine/src/SparseArray.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>


namespace RType {
    namespace Components {

        class HealthViewer {
            public:
                HealthViewer(uint16_t maxhp);
                ~HealthViewer(void) = default;

                static void componentFunction(Rengine::ECS& ecs, RType::Components::HealthViewer& health_view, Rengine::Entity& entity);

            private:
                uint16_t _maxhp;
                std::shared_ptr<Rengine::Graphics::ASprite> _redBarSprite;
                std::shared_ptr<Rengine::Graphics::ASprite> _greenBarSprite;

        };  // class HealthViewer
    }  // namespace Components
}  // namespace RType