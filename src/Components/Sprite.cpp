//
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Graphics/AWindow.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>

#include "../Config/EntityConfig.hpp"
#include "Position.hpp"
#include "Sprite.hpp"

namespace RType {
    namespace Components {
        Sprite::Sprite(const Rengine::Graphics::SpriteSpecs& spriteConfig)
        {
            Rengine::Graphics::GraphicManager& graphicManager = Rengine::Graphics::GraphicManagerSingletone::get();

            this->_sprite = graphicManager.createSprite(spriteConfig);
            if (this->_sprite == nullptr) {
                throw SpriteException("Got nullptr when trying to create a sprite.");
            }
        }

        std::shared_ptr<Rengine::Graphics::ASprite>& Sprite::getSprite(void) noexcept
        {
            return this->_sprite;
        }
        void Sprite::renderSprite(const Rengine::Graphics::vector2D<float> &position)
        {
            Rengine::Graphics::GraphicManager& graphicManager = Rengine::Graphics::GraphicManagerSingletone::get();
            std::shared_ptr<Rengine::Graphics::AWindow>& window = graphicManager.getWindow();

            window->addSpriteToRender(this->_sprite, position);
        }

        void Sprite::componentFunction(Rengine::ECS& ecs, Sprite& sprite, Rengine::Entity& entity)
        {
            std::optional<std::reference_wrapper<Position>> pos = entity.getComponentNoExcept<Position>();

            if (pos.has_value() == false) {
                std::cout << "[Sprite] Entity " << int(entity) << "has no position component" << std::endl;
                return;
            }
            sprite.renderSprite(pos.value().get().getVector2D());
        }
    }  // namespace Components
}  // namespace RType
