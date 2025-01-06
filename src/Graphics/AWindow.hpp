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
        * @class AWindow
        * @brief The window class.
        */
        class AWindow {
            public:
                virtual ~AWindow(void) = default;
                /**
                * @fn addSpriteToRender
                * @param sprite The sprite to render.
                * @param position The window position to render the sprite.
                * @param updateAnimationFrame Whether or not the window should update the sprite's animation before rendering. True by default.
                * @brief Add a sprite to the render.
                */
                virtual void addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite,
                    const Rengine::Graphics::vector2D<float>& position, bool updateAnimationFrame = true) = 0;
                /**
                * @fn render
                * @brief Render all the sprite added by this->addSpriteToRender.
                */
                virtual void render(void) = 0;
                /**
                * @fn isOpen
                * @return True if the window is still open, false is the user closed it.
                * @brief Check if the window is open.
                */
                virtual bool isOpen(void) = 0;
                /**
                * @fn close
                * @brief Close the window.
                */
                virtual void close(void) = 0;
                /**
                * @fn getInputManager
                * @return The user input manager.
                * @brief Return a reference to the user input manager.
                */
                Rengine::Graphics::UserInputManager& getInputManager(void) noexcept
                {
                    return this->_inputManager;
                }
                /**
                * @fn pollInput
                * @brief Register the user's input into the input manager.
                */
                virtual void pollInput(void) = 0;
                /**
                * @fn getElapsedTimeMicroseconds
                * @return The number of microseconds since the window was created.
                * @brief Return the number of microseconds since creation.
                */
                virtual uint64_t getElapsedTimeMicroseconds(void) const noexcept = 0;
                /**
                * @fn getElapsedTimeSeconds
                * @return The number of seconds since the window was created.
                * @brief Return the number of seconds since creation.
                */
                virtual float getElapsedTimeSeconds(void) const noexcept = 0;

            protected:
                UserInputManager _inputManager;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
