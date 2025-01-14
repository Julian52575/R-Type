#include "HitBoxViewer.hpp"
#include "Hitbox.hpp"
#include "Position.hpp"

namespace RType {
    namespace Components {
        HitboxViewer::HitboxViewer(float width, float height){
            Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
            Rengine::Graphics::SpriteSpecs spriteSpecs;

            spriteSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
            spriteSpecs.color = {0, 0, 0};
            spriteSpecs.opacity = 0;
            spriteSpecs.shapeData.outlineColor = {255, 0, 0};
            spriteSpecs.shapeData.outlineThickness = 2;
            spriteSpecs.shapeData.specifics.rectangleSize = {width, height};
            this->_hitboxSprite = manager.createSprite(spriteSpecs);
        }

        HitboxViewer::~HitboxViewer(void){}

        void HitboxViewer::componentFunction(Rengine::ECS& ecs, RType::Components::HitboxViewer& view, Rengine::Entity& entity)
        {
            std::optional<std::reference_wrapper<Hitbox>> hitbox = entity.getComponentNoExcept<Hitbox>();
            std::optional<std::reference_wrapper<Position>> pos = entity.getComponentNoExcept<Position>();

            if (!hitbox || !pos) {
                return;
            }
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(view._hitboxSprite, 
            {pos->get().getVector2D().x + hitbox->get().getSpecs().offsetFromSpriteOrigin.x,
                pos->get().getVector2D().y + hitbox->get().getSpecs().offsetFromSpriteOrigin.y});
        }
    }  // namespace Components
}  // namespace RType
        