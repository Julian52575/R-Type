//
#ifndef SRC_CONFIG_ENTITYCONFIGRESOLVER_HPP_
#define SRC_CONFIG_ENTITYCONFIGRESOLVER_HPP_
#include <rengine/Rengine.hpp>

#include "EntityConfig.hpp"

namespace RType {
    namespace Config {

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class EntityConfigResolver
        * @brief Create and store EntityConfig object for faster load.
        */
        class EntityConfigResolver : public Rengine::Resolver::AResolver<std::string, EntityConfig> {
            public:
                EntityConfigResolver(void) = default;
                ~EntityConfigResolver(void) = default;

            private:
                inline EntityConfig createNewElement(const std::string& key) {
                    return EntityConfig(key);
                }
        };  // class EntityConfigResolver

        /**
        * @addtogroup RType::Config
        * @namespace Config
        * @class EntityConfigResolverSingletone
        * @brief A class holding a singletone instance of EntityConfigResolver
        */
        class EntityConfigResolverSingletone {
            public:
                EntityConfigResolverSingletone(void) = default;
                ~EntityConfigResolverSingletone(void) = default;
                /**
                * @fn get
                * @return The singletone instance of EntityConfigResolver.
                * @brief Retrive the singletone instance of EntityConfigResolver
                */
                static EntityConfigResolver& get(void) noexcept {
                    static EntityConfigResolver resolver;

                    return resolver;
                }

        };  // class EntityConfigResolverSingletone

    }  // namespace Config
}  // namespace RType
#endif  // SRC_CONFIG_ENTITYCONFIGRESOLVER_HPP_
