//
#ifndef _SRC_GRAPHICS_ASPRITE_HPP_
#define _SRC_GRAPHICS_ASPRITE_HPP_

#include "SpriteSpecs.hpp"
#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {

        class SpriteException : public std::exception {
            public:
                SpriteException(const std::string& msg) : _msg("Rengine::Graphics::Sprite: " + msg) {};
                const char *what() const noexcept
                {
                    return this->_msg.c_str();
                };

            private:
                std::string _msg;
        };

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class ASprite
        * @brief The sprite class, library independent.
        */
        class ASprite {
            public:
                ASprite(const Rengine::Graphics::SpriteSpecs& spriteSpecs);
                virtual ~ASprite(void) = default;
                const Rengine::Graphics::SpriteSpecs& getSpriteSpecs(void) const noexcept;
                virtual void advanceFrame(const int16_t frameCount) = 0;
                virtual void setSpriteSpecs(const Rengine::Graphics::SpriteSpecs& spriteSpecs) = 0;

            protected:
                Rengine::Graphics::SpriteSpecs _spriteSpecs;
                uint16_t _currentFrame = 0;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
