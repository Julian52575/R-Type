//
#ifndef SRC_COMPONENTS_SPRITE_HPP_
#define SRC_COMPONENTS_SPRITE_HPP_
#include <cstddef>
#include <memory>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Graphics/ASprite.hpp>

#include "../Config/EntityConfig.hpp"

namespace RType {
    namespace Components {
        class SpriteException : public std::exception {
            public:
                explicit SpriteException(const std::string& msg)
                {
                    this->_msg = "RType::Component::Sprite: " + msg;
                }
                ~SpriteException(void) = default;
                const char *what() const noexcept {
                    return this->_msg.c_str();
                }

            private:
                std::string _msg;
        };

        class Sprite {
            public:
                Sprite(void) = default;
                explicit Sprite(Rengine::Graphics::GraphicManager& graphicManager, const Rengine::Graphics::SpriteSpecs& spriteConfig);
                ~Sprite(void) = default;
                std::shared_ptr<Rengine::Graphics::ASprite>& getSprite(void) noexcept;
                void renderSprite(const Rengine::Graphics::vector2D<float> &position);
                void advanceFrame(const int16_t frame);

            private:
                std::shared_ptr<Rengine::Graphics::AWindow> _window = nullptr;
                std::shared_ptr<Rengine::Graphics::ASprite> _sprite = nullptr;
        };
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_SPRITE_HPP_
