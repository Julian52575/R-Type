//
#ifndef SRC_COMPONENTS_SPRITE_HPP_
#define SRC_COMPONENTS_SPRITE_HPP_
#include <cstddef>
#include <memory>
#include <string>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>

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
                explicit Sprite(const Rengine::Graphics::SpriteSpecs& spriteConfig);
                ~Sprite(void) = default;
                std::shared_ptr<Rengine::Graphics::ASprite>& getSprite(void) noexcept;
                void renderSprite(const Rengine::Graphics::vector2D<float> &position);
                static void componentFunction(Rengine::ECS& ecs, Sprite& sprite, Rengine::Entity& entity);

            private:
                std::shared_ptr<Rengine::Graphics::ASprite> _sprite = nullptr;
                std::shared_ptr<Rengine::Graphics::AText> id_text = nullptr;
        };
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_SPRITE_HPP_
