//
#include <cstdint>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Time.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <memory>

#include "../SoundSpecs.hpp"
#include "SFMLSound.hpp"

namespace Rengine {
    namespace Graphics {

        void SFMLSound::setLoopPoint(const Rengine::Graphics::vector2D<uint64_t>& loopPointsMicroseconds)
        {
            int64_t convertX = 0;
            int64_t convertY = 0;

            if (loopPointsMicroseconds.x > INT32_MAX) {
                convertX = INT32_MAX;
            } else {
                convertX = loopPointsMicroseconds.x;
            }
            if (loopPointsMicroseconds.y > INT32_MAX) {
                convertY = INT32_MAX;
            } else {
                convertY = loopPointsMicroseconds.y;
            }
            sf::Time loopStartMicroseconds = sf::microseconds(convertX);
            sf::Time loopEndMicroseconds = sf::microseconds(convertY);

            // Loop end after music length
            if (loopEndMicroseconds >= this->_music.getDuration()) {
                loopEndMicroseconds = sf::microseconds(this->_music.getDuration().asMicroseconds() - 1);
            }
            // Start after end
            if (loopStartMicroseconds >= loopEndMicroseconds) {
                loopStartMicroseconds = sf::microseconds(loopEndMicroseconds.asMicroseconds() - 1);
            }
            sf::Music::Span<sf::Time> sfLoopPoints(loopStartMicroseconds, loopEndMicroseconds);
            this->_music.setLoopPoints(sfLoopPoints);
        }

        SFMLSound::SFMLSound(const SoundSpecs& specs)
            : ASound(specs)
        {
            std::ifstream f;

            // Reading and storing complete file
            try {
                // Storing and checking file size
                this->_musicSize = std::filesystem::file_size(specs.soundPath);
                if (this->_musicSize == 0) {
                    throw std::runtime_error("Empty music file");
                }
                f.open(specs.soundPath);
                // Allocating musicSize bytes
                std::allocator<uint8_t> alloc;

                this->_musicData = alloc.allocate(this->_musicSize);
                // Reading and creating the sfMusic
                f.read((char *) this->_musicData, this->_musicSize);
                this->_music.openFromMemory(this->_musicData, this->_musicSize);
            } catch (std::exception& e) {
                std::string msg = e.what();

                throw SoundException(msg);
            }
            // Setup specs
            this->_music.setVolume(specs.volume);
            this->_music.setLoop(specs.loop);
            // setLoopPoints
            if (specs.loopPointsMicroseconds != Rengine::Graphics::vector2D<uint64_t>({0, 0})) {
                this->setLoopPoint(specs.loopPointsMicroseconds);
            }
skipSetLoopPoints:
            this->_music.setPitch(specs.pitch);
            this->_music.setPlayingOffset(sf::microseconds(specs.playingOffsetMicroseconds));
        }

        SFMLSound::~SFMLSound(void)
        {
            std::allocator<uint8_t> alloc;

            if (this->_musicData != nullptr) {
                alloc.deallocate(this->_musicData, this->_musicSize);
            }
        }
        sf::Music& SFMLSound::getMusic(void) noexcept
        {
            return this->_music;
        }
        void SFMLSound::play(void)
        {
            this->_music.play();
        }
        void SFMLSound::pause(void)
        {
            this->_music.pause();
        }
        bool SFMLSound::isPlaying(void) const noexcept
        {
            if (this->_music.getStatus() == sf::Music::Status::Playing) {
                return true;
            }
            return false;
        }
        void SFMLSound::reset(void)
        {
            this->_music.stop();
        }
        void SFMLSound::setVolume(float volume)
        {
            this->_music.setVolume(volume);
        }
        float SFMLSound::getVolume(void) const noexcept
        {
            return this->_music.getVolume();
        }
    }  // namespace Graphics
}  // namespace Rengine
