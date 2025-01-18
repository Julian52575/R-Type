//
#include <nlohmann/json.hpp>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <string>

#include "ImageConfig.hpp"

namespace Undertale {

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
            } catch (std::exception &e) {
                throw ImageConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
            if (j.contains("image") == false) {
                throw ImageConfigExceptionInvalidJsonFile(jsonPath, "No 'image' field.");
            }
            j = j["image"];  // Get image general field
            this->parseImage(j);
        }
        ImageConfig::ImageConfig(const std::string& jsonPath, const std::string& texturePath)
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
            } catch (std::exception &e) {
                throw ImageConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
            if (j.contains("image") == false) {
                throw ImageConfigExceptionInvalidJsonFile(jsonPath, "No 'image' field.");
            }
            j = j["image"];  // Get image general field
            this->parseImage(j);
            this->_data.texturePath = texturePath;
        }
        const Rengine::Graphics::SpriteSpecs& ImageConfig::getSpecs(void) const noexcept
        {
            return this->_data;
        }
        void ImageConfig::parseImage(nlohmann::json& imageField)
        {
            // TextureScale
            if (imageField.contains("scale") == true) {
                nlohmann::json& scaleField = imageField["scale"];

                // x && y
                if (scaleField.contains("x") == true) {
                    this->_data.textureScale.x = scaleField["x"];
                }
                if (scaleField.contains("y") == true) {
                    this->_data.textureScale.y = scaleField["y"];
                }
            }
            // OriginOffset
            if (imageField.contains("origin") == true) {
                nlohmann::json& originField = imageField["origin"];

                // x && y
                if (originField.contains("x") == true) {
                    this->_data.originOffset.x = originField["x"];
                }
                if (originField.contains("y") == true) {
                    this->_data.originOffset.y = originField["y"];
                }
            }
            if (imageField.contains("texturePath") == true) {
                this->_data.texturePath = imageField["texturePath"];
            }
            // Animation
            if (imageField.contains("animations") == true) {
                nlohmann::json animationField = imageField["animations"];

                this->_data.animation = Rengine::Graphics::SpriteSpecs::SpriteAnimationSpecs();
                // frameRect
                if (animationField.contains("frameRect") == true) {
                    nlohmann::json& frameRectField = animationField["frameRect"];

                    // x && y
                    if (frameRectField.contains("x") == true) {
                        this->_data.animation->frameRectXY.x = frameRectField["x"];
                    }
                    if (frameRectField.contains("y") == true) {
                        this->_data.animation->frameRectXY.y = frameRectField["y"];
                    }
                    // width && height
                    if (frameRectField.contains("width") == true) {
                        this->_data.animation->frameRectWidthHeight.x = frameRectField["width"];
                    }
                    if (frameRectField.contains("height") == true) {
                        this->_data.animation->frameRectWidthHeight.y = frameRectField["height"];
                    }
                }  // frameRect
                // frameCount
                if (animationField.contains("frameCount") == true) {
                    this->_data.animation->frameCount = animationField["frameCount"];
                }
                // frameDuration
                if (animationField.contains("frameDuration") == true) {
                    this->_data.animation->frameDuration = animationField["frameDuration"];
                }
                // frameDisplacement
                if (animationField.contains("frameDisplacement") == true) {
                    nlohmann::json& frameDisplacementField = animationField["frameDisplacement"];

                    // x && y
                    if (frameDisplacementField.contains("x") == true) {
                        this->_data.animation->frameDisplacement.x = frameDisplacementField["x"];
                    }
                    if (frameDisplacementField.contains("y") == true) {
                        this->_data.animation->frameDisplacement.y = frameDisplacementField["y"];
                    }
                }
            }
        }
    }  // namespace Config
}  // namespace Undertale
