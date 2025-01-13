//
#ifndef SRC_CONFIG_IMAGECONFIGRESOLVER_HPP_
#define SRC_CONFIG_IMAGECONFIGRESOLVER_HPP_
#include <rengine/Rengine.hpp>
#include <string>

#include "ImageConfig.hpp"

namespace RType {
    namespace Config {

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class ImageConfigResolver
        * @brief Create and store ImageConfig object for faster load.
        */
        class ImageConfigResolver : public Rengine::Resolver::AResolver<std::string, ImageConfig> {
            public:
                ImageConfigResolver(void) = default;
                ~ImageConfigResolver(void) = default;

            private:
                inline ImageConfig createNewElement(const std::string& key) {
                    return ImageConfig(key);
                }
        };  // class ImageConfigResolver

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class ImageConfigResolverSingletone
        * @brief A class holding a singletone instance of ImageConfigResolver
        */
        class ImageConfigResolverSingletone {
            public:
                ImageConfigResolverSingletone(void) = default;
                ~ImageConfigResolverSingletone(void) = default;
                /**
                * @fn get
                * @return The singletone instance of ImageConfigResolver.
                * @brief Retrive the singletone instance of ImageConfigResolver
                */
                static ImageConfigResolver& get(void) noexcept {
                    static ImageConfigResolver resolver;

                    return resolver;
                }
        };  // class ImageConfigResolverSingletone
    }  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_IMAGECONFIGRESOLVER_HPP_
