//
#ifndef SRC_GRAPHICS_SFML_SFMLSOUND_HPP_
#define SRC_GRAPHICS_SFML_SFMLSOUND_HPP_

#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Time.hpp>
#include <cstddef>
#include <cstdint>

#include "../SoundSpecs.hpp"
#include "../ASound.hpp"

namespace Rengine {
    namespace Graphics {
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class SFMLSound
        * @brief Implementation of ASound using the SFML library.
        * See ASound.
        */
        class SFMLSound : public ASound {
            public:
                explicit SFMLSound(const SoundSpecs& specs);
                ~SFMLSound(void);
                sf::Music& getMusic(void) noexcept;
                void play(void);
                bool isPlaying(void) const noexcept;
                void pause(void);
                void reset(void);
                void setVolume(float volume);
                float getVolume(void) const noexcept;

            private:
                void setLoopPoint(const Rengine::Graphics::vector2D<uint64_t>& loopPoints);
                uint8_t *_musicData = nullptr;
                uintmax_t _musicSize = 0;
                sf::Music _music;
        };

    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_SFML_SFMLSOUND_HPP_
