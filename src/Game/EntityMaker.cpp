//
#include <cstddef>
#include <rengine/Rengine.hpp>
#include <rengine/src/ECS.hpp>
#include <string>

#include "EntityMaker.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Configuration.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Components/HitboxViewer.hpp"
#include "src/Components/Velocity.hpp"
#include "src/Components/Chrono.hpp"
#include "src/Components/Life.hpp"
#include "src/Components/Clickable.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Components/HealthViewer.hpp"
#include "src/Config/EntityConfig.hpp"
#include "src/Config/EntityConfigResolver.hpp"
#include "src/Game/Team.hpp"
#include "src/Config/ConfigurationIdResolver.hpp"

namespace RType {


        Rengine::Entity& EntityMaker::make(Rengine::ECS& ecs, const RType::Config::EntityConfig& enConfig, uint64_t group)
        {
            Rengine::Entity& entity = ecs.addEntity();

            entity.addComponent<RType::Components::Configuration>(enConfig);
            entity.addComponent<RType::Components::Position>(0, 0);
            entity.addComponent<RType::Components::Buff>();
            entity.addComponent<RType::Components::Hitbox>(enConfig.getHitbox());
            entity.addComponent<RType::Components::Metadata>();
            entity.addComponent<RType::Components::Life>(enConfig.getStats().hp);
            RType::Components::Relationship relation = entity.addComponent<RType::Components::Relationship>();

            relation.setGroup(group);
            entity.setComponentsDestroyFunction(
               [](Rengine::Entity& en) {
                    en.removeComponentNoExcept<RType::Components::Configuration>();
                    en.removeComponentNoExcept<RType::Components::Position>();
                    en.removeComponentNoExcept<RType::Components::Buff>();
                    en.removeComponentNoExcept<RType::Components::Hitbox>();
                    en.removeComponentNoExcept<RType::Components::Metadata>();
                    en.removeComponentNoExcept<RType::Components::Life>();
                    en.removeComponentNoExcept<RType::Components::Relationship>();
                }
            );
            return entity;
        }

        Rengine::Entity& EntityMaker::make(Rengine::ECS& ecs, const std::string& json, uint64_t group, RType::Config::EntityConfig* configPtr)
        {
            const Config::EntityConfig& enConfig = Config::EntityConfigResolverSingletone::get().get(json);

            if (configPtr != nullptr) {
                *configPtr = enConfig;
            }
            return EntityMaker::make(ecs, enConfig, group);
        }

        Rengine::Entity& EntityMaker::make(Rengine::ECS& ecs, uint64_t configurationId, uint64_t group, RType::Config::EntityConfig* configPtr)
        {
            std::string path = RType::Config::EntityConfigurationIdResolverSingletone::get().get(configurationId);

            if (path == "") {
                std::string sId = std::to_string(configurationId);

                throw EntityMakerException("No match for id : " + sId + ".");
            }
            return RType::EntityMaker::make(ecs, path, group, configPtr);
        }
}  // namespace RType
