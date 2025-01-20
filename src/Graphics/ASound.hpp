//
#ifndef SRC_GRAPHICS_ASOUND_HPP_
#define SRC_GRAPHICS_ASOUND_HPP_

#include "SoundSpecs.hpp"
#include <string>

namespace Rengine {
    namespace Graphics {

        class SoundException : public std::exception {
            public:
                explicit SoundException(const std::string& msg) : _msg("Rengine::Graphics::Sound: " + msg) {}
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
        * @class ASound
        * @brief The sound class, library independent.
        */
        class ASound {
            public:
                /**
                * @fn ASound
                * @param soundSpecs The specs of the sound.
                * @brief Create the base class for sound.
                */
                explicit ASound(const SoundSpecs& soundSpecs);
                ~ASound(void) = default;
                /**
                * @fn play
                * @brief Play the sound.
                */
                virtual void play(void) = 0;
                /**
                * @fn play
                * @brief Pause the sound. It can be resumed by using this->play().
                */
                virtual void pause(void) = 0;
                /**
                * @fn isPlaying
                * @return True if the sound is playing, false otherwise.
                * @brief Check is the sound is currently playing.
                */
                virtual bool isPlaying(void) const noexcept = 0;
                /**
                * @fn reset
                * @brief Reset the sound to the start.
                */
                virtual void reset(void) = 0;
                /**
                * @fn setVolume
                * @brief Set the sound's volume.
                */
                virtual void setVolume(float volume) = 0;
                /**
                * @fn getVolume
                * @return The sound's volume.
                * @brief Get the sound's volume.
                */
                virtual float getVolume(void) const noexcept = 0;

            private:
                SoundSpecs _specs;
        };

    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_ASOUND_HPP_
