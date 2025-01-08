//
#ifndef SRC_CONFIG_LEVELCONFIGRESOLVER_HPP_
#define SRC_CONFIG_LEVELCONFIGRESOLVER_HPP_
#include <rengine/Rengine.hpp>
#include <string>

#include "LevelConfig.hpp"

namespace RType {
    namespace Config {

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class LevelConfigResolver
        * @brief Create and store LevelConfig object for faster load.
        */
        class LevelConfigResolver : public Rengine::Resolver::AResolver<std::string, LevelConfig> {
            public:
                LevelConfigResolver(void) = default;
                ~LevelConfigResolver(void) = default;

            private:
                inline LevelConfig createNewElement(const std::string& key) {
                    return LevelConfig(key);
                }
        };  // class LevelConfigResolver

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class LevelConfigResolverSingletone
        * @brief A class holding a singletone instance of LevelConfigResolver
        */
        class LevelConfigResolverSingletone {
            public:
                LevelConfigResolverSingletone(void) = default;
                ~LevelConfigResolverSingletone(void) = default;
                /**
                * @fn get
                * @return The singletone instance of LevelConfigResolver.
                * @brief Retrive the singletone instance of LevelConfigResolver
                */
                static LevelConfigResolver& get(void) noexcept {
                    static LevelConfigResolver resolver;

                    return resolver;
                }
        };  // class LevelConfigResolverSingletone

    }  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_LEVELCONFIGRESOLVER_HPP_
