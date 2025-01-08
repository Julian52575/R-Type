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
                /**
                * @fn ASprite
                * @param spriteSpecs The specs of the sprite.
                * @param creationTimeMicroseconds The time of creation in microseconds.
                * @brief Create the base class for sprites.
                */
                ASprite(const Rengine::Graphics::SpriteSpecs& spriteSpecs, uint64_t creationTimeMicroseconds);
                virtual ~ASprite(void) = default;
                /**
                * @fn getSpriteSpecs
                * @return SpriteSpecs A const reference to the specs used to create this sprite.
                * @brief Return the specs of the sprite.
                */
                const Rengine::Graphics::SpriteSpecs& getSpriteSpecs(void) const noexcept;
                /**
                * @fn advanceFrame
                * @param frameCount The number of frame to advance the sprite's animation.
                * @brief Advance the sprite's animation by the requested amount. 1 by default
                * Note: 0 does nothing
                */
                virtual void advanceFrame(int16_t frameCount = 1) = 0;
                /**
                * @fn advanceFrameFromTime
                * @param currentTimeMicroseconds The current time in microseconds.
                * @brief Advance the sprite's animation depending on the current time.
                */
                virtual void advanceFrameFromTime(uint64_t currentTimeMicroseconds) = 0;
                /**
                * @fn hasAnimation
                * @return True is the sprite has animation data, false otherwise
                * @brief Get whether or not this sprite has animations.
                */
                bool hasAnimation(void) const noexcept;

            protected:
                Rengine::Graphics::SpriteSpecs _spriteSpecs;
                uint8_t _currentFrame = 0;
                uint64_t _creationTimeMicroseconds = 0;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
