//
#ifndef _SRC_GRAPHICS_IWINDOW_HPP_
#define _SRC_GRAPHICS_IWINDOW_HPP_

#include <cstdint>
#include <memory>

#include "AText.hpp"
#include "ASprite.hpp"
#include "Vector.hpp"
#include "UserInputManager.hpp"
#include "WindowSpecs.hpp"

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
                /**
                * @fn AWindow
                * @param windowSpecs The specs of the to-be-created window
                * @param maxInputPerFrame The max number of input to be processed per frame
                * @brief Create the bass class of windows.
                */
                AWindow(const WindowSpecs& windowSpecs, uint16_t maxInputPerFrame = 100) : _windowSpecs(windowSpecs), _inputManager(maxInputPerFrame) {}
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
                * @fn addTextToRender
                * @param text The sprite to render.
                * @param position The window position to render the text.
                * @brief Add a text to the render.
                */
                virtual void addTextToRender(const std::shared_ptr<Rengine::Graphics::AText>& text,
                    const Rengine::Graphics::vector2D<float>& position) = 0;
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
                * @fn getSpecs
                * @return The specs of the window.
                * @brief Return the specs used to create the window.
                */
                const Rengine::Graphics::WindowSpecs& getSpecs(void) const noexcept
                {
                    return this->_windowSpecs;
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
                /**
                * @fn resetDeltatime
                * @brief Resets the deltatime timer to 0.
                */
                virtual void resetDeltatime(void) noexcept = 0;
                /**
                * @fn getDeltaTimeMicroseconds
                * @return The number of microseconds since the last deltatime reset.
                * @brief Retrives the microseconds since last deltatime reset.
                */
                virtual uint64_t getDeltaTimeMicroseconds(void) noexcept = 0;
                /**
                * @fn getDeltaTimeSeconds
                * @return The number of seconds since the last deltatime reset.
                * @brief Retrives the seconds since last deltatime reset.
                */
                virtual float getDeltaTimeSeconds(void) noexcept = 0;

            protected:
                WindowSpecs _windowSpecs;
                UserInputManager _inputManager;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
