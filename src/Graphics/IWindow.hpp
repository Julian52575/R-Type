//
#ifndef _SRC_GRAPHICS_IWINDOW_HPP_
#define _SRC_GRAPHICS_IWINDOW_HPP_
#include "ASprite.hpp"
#include "Vector.hpp"

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
        class IWindow {
            public:
                virtual ~IWindow(void) = default;
                virtual void addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite, const Rengine::Graphics::vector2D<float>& position) = 0;
                virtual void render(void) = 0;
                virtual bool isOpen(void) = 0;
                virtual void close(void) = 0;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
