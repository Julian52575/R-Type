//
#ifndef SRC_CONFIG_IMAGECONFIG_HPP_
#define SRC_CONFIG_IMAGECONFIG_HPP_
#include <rengine/Rengine.hpp>
#include <string>
#include <nlohmann/json.hpp>
#include <rengine/src/Graphics/SpriteSpecs.hpp>

namespace RType {

    namespace Config {

        class ImageConfigExceptionFileError : public std::exception {
        public:
            ImageConfigExceptionFileError(const std::string& fileName, const char *jsonMsg)
            {
                std::string msg = "RType::Config::ImageConfig An error happened when trying to read from '" + fileName + "'.";

                this->_concat = msg + jsonMsg;
            };
            const char *what() const noexcept
            {
                return this->_concat.c_str();
            };

        private:
            std::string _concat;
        };
        class ImageConfigExceptionInvalidJsonFile : public std::exception {
        public:
            ImageConfigExceptionInvalidJsonFile(const std::string& fileName, const char *jsonMsg)
            {
                std::string msg = "RType::Config::ImageConfig Trying to construct from an invalid json file '" + fileName + "'.\nError: '";

                this->_concat = msg + jsonMsg + "'.";
            };
            const char *what() const noexcept
            {
                return this->_concat.c_str();
            };

        private:
            std::string _concat;
        };

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class ImageConfig
        * @brief This class encapsulate the image json.
        * Is used to convert a jsonPath to a class of c++ variable.
        */
        class ImageConfig {
            public:
                /**
                * @fn ImageConfig
                * @brief Create an empty ImageConfig.
                */
                ImageConfig(void) = default;
                /**
                * @fn ImageConfig
                * @param jsonPath The path to a json image configuration.
                * The image configuration must start with the 'image' field.
                */
                explicit ImageConfig(const std::string& jsonPath);
                ~ImageConfig(void) = default;
                /**
                * @fn getConfig
                * @return Rengine::Graphics::SpriteSpecs A structure containing the image configuration.
                * @brief Return the configuration of the image.
                */
                const Rengine::Graphics::SpriteSpecs& getSpecs(void) const noexcept;

            private:
                void parseImage(nlohmann::json& imageField);
                Rengine::Graphics::SpriteSpecs _data;
        };  // class ImageConfig
    }  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_IMAGECONFIG_HPP_
