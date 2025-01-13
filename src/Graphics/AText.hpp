//
#ifndef SRC_GRAPHICS_ATEXT_HPP_
#define SRC_GRAPHICS_ATEXT_HPP_
#include <string>

#include "TextSpecs.hpp"
namespace Rengine {
    namespace Graphics {

        class TextException : public std::exception {
            public:
                TextException(const std::string& msg) : _msg("Rengine::Graphics::Text: " + msg) {};
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
        * @class AText
        * @brief The text class, library independent.
        */
        class AText {
            public:
                AText(const TextSpecs& specs);
                ~AText() = default;
                /**
                * @fn setText
                * @param text The new to be set
                * @brief Overwrite the current text with the new one.
                */
                virtual void setText(const std::string& text) = 0;
                /**
                * @fn setText
                * @return The currently set text.
                * @brief Get the current text.
                */
                virtual const std::string& getText(void) const noexcept = 0;

            private:
                TextSpecs _specs;
        };
    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_ATEXT_HPP_
