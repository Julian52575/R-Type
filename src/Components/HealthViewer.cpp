#include "HealthViewer.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Configuration.hpp"


namespace RType{

    namespace Components {

        HealthViewer::HealthViewer(uint16_t maxhp) : _maxhp(maxhp) {
            Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
            Rengine::Graphics::SpriteSpecs spriteSpecs;

            spriteSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
            spriteSpecs.color = {0, 0, 0};
            spriteSpecs.opacity = 0;
            spriteSpecs.shapeData.outlineColor = {255, 0, 0};
            spriteSpecs.shapeData.outlineThickness = 2;
            spriteSpecs.shapeData.specifics.rectangleSize = {100, 10};
            this->_redBarSprite = manager.createSprite(spriteSpecs);

            spriteSpecs.shapeData.outlineColor = {0, 255, 0};
            this->_greenBarSprite = manager.createSprite(spriteSpecs);
        }

        void HealthViewer::componentFunction(Rengine::ECS& ecs, RType::Components::HealthViewer& health_view, Rengine::Entity& entity){
            std::optional<std::reference_wrapper<Position>> pos = entity.getComponentNoExcept<Position>();
            std::optional<std::reference_wrapper<Configuration>> config = entity.getComponentNoExcept<Configuration>();

            if (!pos || !config) {
                return;
            }
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(health_view._redBarSprite,pos->get().getVector2D());
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(health_view._greenBarSprite,pos->get().getVector2D());
        }

    }
}