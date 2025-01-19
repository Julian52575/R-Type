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

            Rengine::Graphics::TextSpecs specs;
            specs.style = Rengine::Graphics::TextStyle::TextStyleBold;
            specs.color = {0, 0, 255};
            specs.fontPath = "assets/fonts/arial.ttf";
            // specs.message = std::to_string(int(entity));

            this->id_text = Rengine::Graphics::GraphicManagerSingletone::get().createText(specs);
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

            sprite.id_text.get()->setText(std::to_string(int(entity)));
            sprite.renderSprite(pos.value().get().getVector2D());

        #ifdef _DEBUG
            Rengine::Graphics::vector2D<float> posText = pos.value().get().getVector2D();
            posText.y -= 20;
            Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->addTextToRender(sprite.id_text, posText);
        #endif
        }
    }  // namespace Components
}  // namespace RType
