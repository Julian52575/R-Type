//
#ifndef _SRC_GRAPHICS_WINDOWSPECS_HPP_
#define _SRC_GRAPHICS_WINDOWSPECS_HPP_
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {

        class WindowSpecsException : public std::exception {
            public:
                WindowSpecsException(const std::string& msg) : _msg("Rengine::Graphics::WindowSpecs: " + msg) {};
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
        * @class WindowSpecs
        * @brief A structure of the specs needed to create a window.
        */
        struct WindowSpecs {
            /**
            * @fn buildFromJson
            * @param jsonPath The path to the json to read.
            * @brief Build this structure's field from the provided json file.
            */
            void buildFromJson(const std::string& jsonPath);
            /**
            * @brief The title of the window.
            */
            std::string title = "window by Rengine©";
            /**
            * @brief The resolution of the window.
            * Default: 1920x, 1080y
            */
            Rengine::Graphics::vector2D<uint16_t> resolution = {1920, 1080};
            /**
            * @brief The maximum framerate.
            * Default: 60
            */
            uint16_t framerateLimit = 60;
            /**
            * @brief The background color
            * Default: black (0r, 0b, 0g)
            */
            Rengine::Graphics::vector3D<uint8_t> backgroundColor = {0, 0, 0};
            /**
            * @brief The path to the window's icon.
            */
            std::string iconPath;

            /**
            * @addtogroup Rengine::Graphics
            * @namespace Graphics
            * @class WindowOptions
            * @brief A structure of the options for the window.
            */
            struct WindowOption {
                /**
                * @brief Can the window be fullscreened ?
                * default: true
                */
                bool isFullscreen = true;
                /**
                * @brief Can the window be resized ?
                * default: false
                */
                bool isResizable = false;
                /**
                * @brief Is the window borderless ?
                * Default: true
                */
                bool isBorderless = true;
                /**
                * @brief is Vsync enable ?
                * Default: true
                */
                bool enableVsync = true;
                /**
                * @brief Is the cursor visible ?
                */
                bool isCursorVisible = true;
            };  // struct WindowOption
            /**
            * @brief Various options about the window.
            */
            WindowOption options;
            private:
                void parseJsonOptions(nlohmann::json& optionsField);

        };  // struct WindowSpecs

    }  // namespace Graphics
}  //namespace Rengine
#endif
