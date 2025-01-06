//
#ifndef _SRC_GRAPHICS_ASPRITE_HPP_
#define _SRC_GRAPHICS_ASPRITE_HPP_

#include "SpriteSpecs.hpp"
#include "Vector.hpp"
#include <cstdint>

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
                ASprite(const Rengine::Graphics::SpriteSpecs& spriteSpecs, uint64_t _creationTimeMicroseconds);
                virtual ~ASprite(void) = default;
                const Rengine::Graphics::SpriteSpecs& getSpriteSpecs(void) const noexcept;
                virtual void advanceFrame(int16_t frameCount = 1) = 0;
                virtual void advanceFrameFromTime(uint64_t currentTimeMicroseconds) = 0;
                virtual void setSpriteSpecs(const Rengine::Graphics::SpriteSpecs& spriteSpecs) = 0;

            protected:
                Rengine::Graphics::SpriteSpecs _spriteSpecs;
                uint8_t _currentFrame = 0;
                uint64_t _creationTimeMicroseconds = 0;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
