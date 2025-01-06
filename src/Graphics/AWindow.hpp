//
#ifndef _SRC_GRAPHICS_IWINDOW_HPP_
#define _SRC_GRAPHICS_IWINDOW_HPP_
#include "ASprite.hpp"
#include "Vector.hpp"
#include "UserInputManager.hpp"

#include <cstdint>
#include <memory>

namespace Rengine {
    namespace Graphics {

        class WindowException : public std::exception {
            public:
                WindowException(const std::string& msg) : _msg("Rengine::Graphics::Window: " + msg) {};
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
        * @class IWindow
        * @brief The window class, library independent.
        */
        class AWindow {
            public:
                virtual ~AWindow(void) = default;
                virtual void addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite,
                    const Rengine::Graphics::vector2D<float>& position, bool updateAnimationFrame = true) = 0;
                virtual void render(void) = 0;
                virtual bool isOpen(void) = 0;
                virtual void close(void) = 0;
                Rengine::Graphics::UserInputManager& getInputManager(void) noexcept
                {
                    return this->_inputManager;
                }
                virtual void pollInput(void) = 0;
                virtual uint64_t getElapsedTimeMicroseconds(void) const noexcept = 0;
                virtual float getElapsedTimeSeconds(void) const noexcept = 0;

            protected:
                UserInputManager _inputManager;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
