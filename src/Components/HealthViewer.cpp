#include "HealthViewer.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Configuration.hpp"
#include "src/Components/Life.hpp"


namespace RType{

    namespace Components {

        HealthViewer::HealthViewer(uint16_t maxhp) : _maxhp(maxhp) {
            Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
            Rengine::Graphics::SpriteSpecs spriteSpecs;

            spriteSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
            spriteSpecs.color = {255, 0, 0};
            spriteSpecs.shapeData.outlineColor = {255, 255, 255};
            spriteSpecs.shapeData.outlineThickness = 2;
            spriteSpecs.shapeData.specifics.rectangleSize = {100, 10};
            this->_redBarSprite = manager.createSprite(spriteSpecs);

            spriteSpecs.color = {0, 255, 0};
            spriteSpecs.shapeData.specifics.rectangleSize = {100, 10};
            this->_greenBarSprite = manager.createSprite(spriteSpecs);
        }

        void HealthViewer::componentFunction(Rengine::ECS& ecs, RType::Components::HealthViewer& health_view, Rengine::Entity& entity){

            std::optional<std::reference_wrapper<Position>> pos = entity.getComponentNoExcept<Position>();
            std::optional<std::reference_wrapper<Life>> life = entity.getComponentNoExcept<Life>();

            if (!pos || !life) {
                return;
            }

            float percent = (float(life->get().getHp()) / float(life->get().getMaxHp()));
            Rengine::Graphics::vector2D<float> size = {100 * percent, 10};

            Rengine::Graphics::SpriteSpecs spriteSpecs = health_view._greenBarSprite.get()->getSpriteSpecs();
            spriteSpecs.shapeData.specifics.rectangleSize = size;
            health_view._greenBarSprite.get()->updateSpriteSpecs(spriteSpecs);
            
            Rengine::Graphics::vector2D<float> pos_with_offset = {pos->get().getVector2D().x - 50, pos->get().getVector2D().y - 50};
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(health_view._redBarSprite,pos_with_offset);
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(health_view._greenBarSprite,pos_with_offset);
        }

    }
}