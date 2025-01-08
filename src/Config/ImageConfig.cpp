//
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

#include "ImageConfig.hpp"

namespace RType {

    namespace Config {

        ImageConfig::ImageConfig(const std::string& jsonPath)
        {
            std::ifstream f;

            try {
                f.open(jsonPath);
            } catch (std::exception &e) {
                throw ImageConfigExceptionFileError(jsonPath, e.what());
            }
            nlohmann::json j;

            try {
                j = nlohmann::json::parse(f);
                j = j["image"];  // Get image general field
                this->parseImage(j);
            }
            // Error on parsing
            catch (std::exception &e) {
                throw ImageConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
        }
        const Rengine::Graphics::SpriteSpecs& ImageConfig::getConfig(void) const noexcept
        {
            return this->_data;
        }
        void ImageConfig::parseImage(nlohmann::json& imageField)
        {
            // TextureScale
            try {
                float scaleX = imageField["scale"]["x"];
                float scaleY = imageField["scale"]["y"];

                this->_data.textureScale = {scaleX, scaleY};
            } catch (std::exception &e) {
                this->_data.textureScale = {1, 1};
            }
            // OriginOffset
            try {
                this->_data.originOffset = {imageField["origin"]["x"], imageField["origin"]["y"]};
            } catch (std::exception &e) {
                this->_data.originOffset = {0, 0};
            }
            this->_data.texturePath = imageField["texturePath"];
            // Animation
            try {
                if (imageField.contains("animations") == false) {
                    return;
                }
                nlohmann::json animationField = imageField["animations"];

                this->_data.animation = Rengine::Graphics::SpriteSpecs::SpriteAnimationSpecs();
                this->_data.animation->frameRectXY = {animationField["frameRect"]["x"], animationField["frameRect"]["y"]};
                this->_data.animation->frameRectWidthHeight = {animationField["frameRect"]["width"], animationField["frameRect"]["height"]};
                this->_data.animation->frameCount = animationField["frameCount"];
                this->_data.animation->frameDuration = animationField["frameDuration"];
                this->_data.animation->frameDisplacement = {animationField["frameDisplacement"]["x"], animationField["frameDisplacement"]["y"]};
            } catch (std::exception &e) {;}
        }
    }  // namespace Config
}  // namespace RType
