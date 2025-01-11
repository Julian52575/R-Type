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

        class AText {
            public:
                AText(const TextSpecs& specs);
                ~AText() = default;

            private:
                TextSpecs _specs;
        };
    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_ATEXT_HPP_
