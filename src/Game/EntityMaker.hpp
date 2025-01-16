//
#include <rengine/Rengine.hpp>

#include "src/Config/EntityConfig.hpp"
namespace RType {

    class EntityMakerException : public std::exception {
        public:
            explicit EntityMakerException(const std::string& msg)
            {
                this->_msg = "RType::EntityMaker: " + msg;
            }
            ~EntityMakerException(void) = default;
            const char *what() const noexcept {
                return this->_msg.c_str();
            }

        private:
            std::string _msg;
    };

    class EntityMaker {
        public:
            /**
            * @fn make
            * @param ecs A reference to the ECS in use.
            * @param config The config to follow.
            * @param group The group the new entity belong to. Default = 0.
            * @brief Create an entity and asign basic components to it.
            */
            static Rengine::Entity& make(Rengine::ECS& ecs, const RType::Config::EntityConfig& config, uint64_t group = 0);
            /**
            * @fn make
            * @param ecs A reference to the ECS in use.
            * @param json The json path to use to create this entity.
            * @param group The group the new entity belong to. Default = 0.
            * @param configPtr A pointer to an EntityConfig to be updated with the parsed configuration.
            * @brief Create an entity and asign basic components to it.
            */
            static Rengine::Entity& make(Rengine::ECS& ecs, const std::string& json, uint64_t group = 0, RType::Config::EntityConfig* configPtr = nullptr);
            /**
            * @fn make
            * @param ecs A reference to the ECS in use.
            * @param configurationId The configuration id to load.
            * @param group The group the new entity belong to. Default = 0.
            * @param configPtr A pointer to an EntityConfig to be updated with the parsed configuration.
            * @exception EntityMakerException The configurationId is invalid.
            * @brief Create an entity and asign basic components to it.
            */
            static Rengine::Entity& make(Rengine::ECS& ecs, uint64_t configurationId, uint64_t group = 0, RType::Config::EntityConfig* configPtr = nullptr);
    };

}  // namespace RType
