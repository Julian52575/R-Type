//
#ifndef SRC_GRAPHICS_SOUNDSPECS_HPP_
#define SRC_GRAPHICS_SOUNDSPECS_HPP_

#include "Vector.hpp"
#include <string>

namespace Rengine {
    namespace Graphics {

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class SoundSpecs
        * @brief A structure of the specs needed to create a sound / music.
        */
        struct SoundSpecs {
            /**
            * @brief The path of the sound file to be opened.
            */
            std::string soundPath = "";
            /**
            * @brief The volume of the sound.
            */
            float volume = 80;
            /**
            * @brief Should the sound restart after it ended ?
            */
            bool loop = false;
            /**
            * @brief If nozero, set the start and the end of the loop.
            * Default: {0, 0}
            */
            Rengine::Graphics::vector2D<uint64_t> loopPointsMicroseconds = {0, 0};
            /**
            * @brief The microseconds offset where this sound should start playing.
            */
            uint64_t playingOffsetMicroseconds = 0;
            /**
            * @brief The sound pitch. A higher value means the sound will be more acute.
            */
            float pitch = 1;
        };

    }  // namespace Rengine
}  // namespace Graphics

#endif  // SRC_GRAPHICS_SOUNDSPECS_HPP_
