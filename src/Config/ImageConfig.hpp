//
#ifndef _SRC_CONFIG_IMAGECONFIG_HPP_
#define _SRC_CONFIG_IMAGECONFIG_HPP_
#include <nlohmann/json.hpp>

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
        * @class ImageConfigData
        * @brief A structure of the fields specified inside the image json.
        */
        struct ImageConfigData {
            std::string texturePath;
            std::pair<uint16_t, uint16_t> originOffset;
            std::pair<float, float> textureScale;

            struct EntityConfigSpriteAnimation {
                std::pair<uint16_t, uint16_t> frameRectXY;
                std::pair<uint16_t, uint16_t> frameRectWidthHeight;
                uint16_t frameCount;
                float frameDuration;
                std::pair<uint16_t, uint16_t> frameDisplacement;
            };
            EntityConfigSpriteAnimation animation;
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
                * @brief Do not call this default constructor, it will create an empty ImageConfig.
                */
                ImageConfig(void) = default;
                /**
                * @fn ImageConfig
                * @param jsonPath The path to a json image configuration.
                * The image configuration must start with the 'image' field.
                */
                ImageConfig(const std::string& jsonPath);
                ~ImageConfig(void) = default;
                /**
                * @fn getConfig
                * @return RType::Config::ImageConfigData A structure containing the image configuration.
                * @brief Return the configuration of the image.
                */
                const ImageConfigData& getConfig(void) const noexcept;

            private:
                void parseImage(nlohmann::json& imageField);
                ImageConfigData _data;

        };  // class ImageConfig
    }  // namespace Config
}  // namespace RType
#endif
