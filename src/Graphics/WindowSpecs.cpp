//
#include <cstdint>
#include <exception>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Vector.hpp"
#include "WindowSpecs.hpp"

namespace Rengine {
    namespace Graphics {


/* Example json:
{
    "title": "window",
    "resolution": {
        "width": 800,
        "height": 600
    },
    "framerateLimit": 60,
    "backgroundColor": {
        "r": 0,
        "g": 0,
        "b": 0
    },
    "icon": "assets/icon.png",
    "options": {
        "isFullscreen": false,
        "isResizable": true,
        "isBorderless": false,
        "enableVsync": true,
        "isCursorVisible": true
    }
}

*/

        void WindowSpecs::buildFromJson(const std::string& jsonPath)
        {
            std::ifstream f;

            try {
                f.open(jsonPath);
            } catch (std::exception &e) {
                std::string msg = jsonPath + ":" + e.what();

                throw WindowSpecsException(msg);
            }
            nlohmann::json j;

            try {
                j = nlohmann::json::parse(f);
            } catch (std::exception& e) {
                std::string msg = jsonPath + ":" + e.what();

                throw WindowSpecsException(msg);
            }
            // Title
            try {
                this->title = j["title"];
            } catch (std::exception& e) {;}
            // Resolution
            try {
                this->resolution.x = j["resolution"]["width"];
                this->resolution.y = j["resolution"]["height"];
            } catch (std::exception& e) {;}
            // framerateLimit
            try {
                this->framerateLimit = j["frameRateLimit"];
            } catch (std::exception& e) {;}
            // backgroundColor
            try {
                this->backgroundColor.x = j["backgroundColor"]["r"];
                this->backgroundColor.y = j["backgroundColor"]["g"];
                this->backgroundColor.z = j["backgroundColor"]["b"];
            } catch (std::exception& e) {;}
            // iconPath
            try {
                this->iconPath = j["iconPath"];
            } catch (std::exception& e) {;}
            // options
            this->parseJsonOptions(j["options"]);
        }
        void WindowSpecs::parseJsonOptions(nlohmann::json& optionsField)
        {
            // isFullscreen
            try {
                this->options.isFullscreen = optionsField["isFullscreen"];
            } catch (std::exception& e) {;}
            // isResizable
            try {
                this->options.isResizable = optionsField["isResizable"];
            } catch (std::exception& e) {;}
            // isBorderless
            try {
                this->options.isBorderless = optionsField["isBorderless"];
            } catch (std::exception& e) {;}
            // enableVsync
            try {
                this->options.enableVsync = optionsField["enableVsync"];
            } catch (std::exception& e) {;}
            // isCursorVisible
            try {
                this->options.isCursorVisible = optionsField["isCursorVisible"];
            } catch (std::exception& e) {;}
        }

    }  // namespace Graphics
}  //namespace Rengine
