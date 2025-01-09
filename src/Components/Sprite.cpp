//
#include <cstddef>
#include <memory>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>

#include "../Config/EntityConfig.hpp"
#include "Sprite.hpp"

namespace RType {
    namespace Components {
        Sprite::Sprite(Rengine::Graphics::GraphicManager& graphicManager, const Rengine::Graphics::SpriteSpecs& spriteConfig)
        {
            this->_window = graphicManager.getWindow();
            if (this->_window == nullptr) {
                throw SpriteException("Got nullptr instead of the window.");
            }
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
            this->_window->addSpriteToRender(this->_sprite, position);
        }
        void Sprite::advanceFrame(const int16_t frame)
        {
            this->_sprite->advanceFrame(frame);
        }
    }  // namespace Components
}  // namespace RType
