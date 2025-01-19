//
#include <chrono>
#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <rengine/src/Clock/Clock.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "src/Game/LuaManager.hpp"
#include "src/Game/EntityMaker.hpp"
#include "src/Network/EntityAction.hpp"
#include "src/Config/AttackBuffTypeEnum.hpp"
#include "src/Config/AttackConfig.hpp"
#include "src/Config/EntityConfig.hpp"
#include "src/Config/EntityConfigResolver.hpp"
#include "src/Config/MissileConfig.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Configuration.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Relationship.hpp"
#include "src/Components/Velocity.hpp"
#include "src/Components/Chrono.hpp"
#include "src/Components/Life.hpp"
#include "src/Game/Team.hpp"

namespace RType {
    namespace Components {

        Action::Action(ActionSource source, const std::string& scriptPath)
        {
            this->_actionSource = source;
            switch (source) {
                case ActionSourceScript:
                    try {
                        this->_luaInfos.id = RType::LuaManagerSingletone::get().loadLuaScript(scriptPath);
                        this->_luaInfos.scriptPath = scriptPath;
                    } catch (std::exception& e) {
                        std::cerr << "[ACTION] Cannot load script " << this->_luaInfos.scriptPath << std::endl;
                        this->_actionSource = ActionSourceNA;
                    }
                    break;


                case ActionSourceUserInput:
                    break;

                default:
                    throw std::runtime_error("RType::Component::Action: Unknow ActionSource.");
            }  // switch source
            this->_actionVector.reserve(5);

            this->_shootDeltatimes[0] = 10000.0f;
            this->_shootDeltatimes[1] = 10000.0f;
            this->_shootDeltatimes[2] = 10000.0f;
        }

        void Action::updateFromSource(Rengine::ECS& ecs, Rengine::Entity& entity) noexcept
        {
            switch (this->_actionSource) {
                case ActionSource::ActionSourceScript:
                    try {
                        this->processLuaScriptInput(ecs, entity);
                    } catch (...) {
                        std::cerr << "Script " << this->_luaInfos.scriptPath << " received execption: " << ". Disabling it..." << std::endl;
                        this->_actionSource = ActionSource::ActionSourceNA;
                        return;
                    }
                    break;

                default:
                    return;
            }
        }

        std::vector<Network::EntityAction> &Action::getActionVector(void) noexcept
        {
            return this->_actionVector;
        }

        ActionSource Action::getActionSource(void) const noexcept
        {
            return this->_actionSource;
        }

        void Action::processAction(const Network::EntityAction& rfcAction) noexcept
        {
            this->_actionVector.push_back(rfcAction);
        }

        void Action::processLuaScriptInput(Rengine::ECS& ecs, Rengine::Entity& entity)
        {
            std::vector<RType::LuaReturn> reply;
            Network::EntityAction act;
            std::optional<std::reference_wrapper<RType::Components::Position>> pos = entity.getComponent<RType::Components::Position>();

            // no pos ? no lua
            if (pos.has_value() == false) {
                return;
            }
            // update deltatime
            RType::LuaManagerSingletone::get().callFunction(this->_luaInfos.scriptPath, this->_luaInfos.id, "updateDeltatime", Rengine::Clock::getElapsedTime());

            // Move
            reply = RType::LuaManagerSingletone::get().callFunction<float, float>(this->_luaInfos.scriptPath, this->_luaInfos.id,
                "move", pos->get().getX(), pos->get().getY());
            // reply is not formatted correcly: ignore
            if (reply.size() < 2) {
                goto shootFunction;
            }
            if (reply[0].type != LuaTypeInt && reply[1].type != LuaTypeInt) {
                goto shootFunction;
            }
            act.data.moveVelocity.x = reply[0].data.integer;
            act.data.moveVelocity.y = reply[1].data.integer;
            act.type = static_cast<Network::EntityActionType>(Network::EntityActionTypeMove);
            this->_actionVector.push_back(act);

shootFunction:
            reply = RType::LuaManagerSingletone::get().callFunction<float, float, float>(this->_luaInfos.scriptPath, this->_luaInfos.id,
                "shoot", this->_shootDeltatimes[0], this->_shootDeltatimes[1], this->_shootDeltatimes[2]);
            // Check reply size and type
            if (reply.size() == 0 || reply[0].type != LuaTypeInt) {
                return;
            }
            act.type = static_cast<Network::EntityActionType>(reply[0].data.integer);
            act.data = {0};
            this->_actionVector.push_back(act);
        }

        Action::const_iterator Action::begin(void) const
        {
            return this->_actionVector.begin();
        }
        Action::const_iterator Action::end(void) const
        {
            return this->_actionVector.end();
        }
        void Action::clear(void)
        {
            this->_actionVector.clear();
        }
        Action::size_type Action::size(void) const noexcept
        {
            return this->_actionVector.size();
        }

        void Action::componentFunction(Rengine::ECS& ecs, Action& actionComponent, Rengine::Entity& entity, std::shared_ptr<Rengine::ECS> &sharedEcs)
        {
            updateDeltatimes(actionComponent);
            std::optional<std::reference_wrapper<Configuration>> entityConfig = entity.getComponentNoExcept<Configuration>();
            std::optional<std::reference_wrapper<Position>> pos = entity.getComponentNoExcept<Position>();
            auto vel = entity.getComponentNoExcept<Velocity>();

            if (entityConfig.has_value() == false || pos.has_value() == false) {
                return;
            }
            actionComponent.updateFromSource(ecs, entity);
            const Rengine::Graphics::vector2D<float>& currentPos = pos->get().getVector2D();
            Rengine::Graphics::vector2D<float> newPos = currentPos;

            for (auto it : actionComponent) {
                // Move
                if (it.type == Network::EntityActionTypeMove && vel.has_value() == true) {
                    actionComponent.handleMove(it, entityConfig.value(), vel->get());
                }
                // Shoot1 -> 3
                if (Network::EntityActionTypeShoot1 <= it.type && it.type <= Network::EntityActionTypeShoot3) {
                    actionComponent.handleShoot(actionComponent, it, sharedEcs, entity, entityConfig.value());
                }
            }  // for it
            actionComponent.clear();
            if (vel.has_value() == true && actionComponent._updatedNonZeroVelocity.x == false) {
                vel->get().setX(0.0f);
            }
            if (vel.has_value() == true && actionComponent._updatedNonZeroVelocity.y == false) {
                vel->get().setY(0.0f);
            }
            actionComponent._updatedNonZeroVelocity = {false, false};
        }

        void updateDeltatimes(Action& component) noexcept
        {
            for (uint8_t i = 0; i < 3; i++) {
                if (component._shootDeltatimes[i] < 10000.0f)
                    component._shootDeltatimes[i] += Rengine::Clock::getElapsedTime();
            }
        }

        void Action::handleMove(Network::EntityAction& action, RType::Components::Configuration& config, RType::Components::Velocity& vel)
        {
            if (action.data.moveVelocity.x != 0.0f) {
                vel.setX(action.data.moveVelocity.x);
                this->_updatedNonZeroVelocity.x = true;
            }
            if (action.data.moveVelocity.y != 0.0f) {
                vel.setY(action.data.moveVelocity.y);
                this->_updatedNonZeroVelocity.y = true;
            }
        }

        void Action::handleShoot(Action& actionComponent, Network::EntityAction& action, std::shared_ptr<Rengine::ECS>& ecs, Rengine::Entity& entity, Configuration& entityConfig)
        {
            // Get attackId by taking advantage of the fact that the 3 TypeShoot follow each other in the enum
            uint8_t attackId = 1 + (action.type - Network::EntityActionTypeShoot1);
            const std::optional<Config::AttackConfig>& attackConfig = entityConfig.getConfig().getAttack(attackId);

            // No attack : no action
            if (attackConfig.has_value() == false) {
                return;
            }
            // Cooldown not reached : no shoot
            if (this->_shootDeltatimes[attackId - 1] < attackConfig->getCooldown()) {
                return;
            }
            this->_shootDeltatimes[attackId - 1] = 0.0f;
            try {
                switch (attackConfig->getType()) {
                    // Handle buffs
                    case (Config::AttackType::AttackTypeBuffs):
                        actionComponent.handleShootBuff(action, ecs, entity, entityConfig, attackConfig);
                        break;
                    // Handle missiles
                    case (Config::AttackType::AttackTypeMissiles):
                        handleShootMissile(actionComponent, action, ecs, entity, entityConfig, attackConfig);
                        break;
                    default:
                        break;
                }
            } catch (std::exception& e) {
                std::cout << "Warning: Got error '" << e.what() << "' on entity shoot'" << std::endl;
            }
        }

        inline void handleShootMissile(Action& actionComponent, Network::EntityAction& action, std::shared_ptr<Rengine::ECS>& ecs,
                        Rengine::Entity& host, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            // Not enough entity left for attack : skip it
            if (attackConfig->getMissiles().size() > ecs->getEntityLimit() - ecs->getActiveEntitiesCount()) {
                return;
            }
            Config::EntityConfigResolver singletone = Config::EntityConfigResolverSingletone::get();
            const Position& hostPosition = host.getComponent<Position>();
            Relationship& hostRelationship = host.getComponent<Relationship>();

            for (const Config::MissileConfig& it : attackConfig->getMissiles()) {
                RType::Config::EntityConfig currentMissileEntityConfig;
                Rengine::Entity& projectile = RType::EntityMaker::make(ecs, it.getJsonPath(), hostRelationship.getGroup(), &currentMissileEntityConfig);
                RType::Components::Relationship& proRelationship = projectile.getComponent<RType::Components::Relationship>();
                uint64_t proID = projectile;
                uint64_t hostId = uint64_t(host);

                // set Components
                projectile.getComponent<Position>().set(
                        {hostPosition.getVector2D().x + (float) it.getOffset().first,
                        hostPosition.getVector2D().y + (float) it.getOffset().second}
                );

                projectile.addComponent<Chrono>([&ecs, proID]() {
                    ecs->removeEntity<>(proID);
                }, 7.0f);
                hostRelationship.addChild(uint64_t(projectile));
                proRelationship.addParent(uint64_t(host));
                proRelationship.setGroup(hostRelationship.getGroup());
                bool hasAction = false;
                bool hasVelocity = false;
                float xVelo = 0.0f;

                switch (it.getControlType()) {
                    case (Config::MissileControlTypeUserInput):
                        projectile.addComponent<Action>(ActionSource::ActionSourceUserInput);
                        hasAction = true;
                        break;

                    case (Config::MissileControlTypeScript):
                        projectile.addComponent<Action>(ActionSource::ActionSourceScript, it.getScriptPath());
                        hasAction = true;
                        break;

                    case (Config::MissileControlTypeVelocity):
                        // Change direction of enemies' projectile
                        xVelo = it.getVelocity().first;

                        if (hostRelationship.belong(Team::TeamEnemy) == true) {
                            xVelo *= -1;
                        }
                        projectile.addComponent<Velocity>(xVelo, it.getVelocity().second);
                        hasVelocity = true;
                        break;

                    // No action component needed for invalid value
                    default:
                        break;
                } // switch controlType
                projectile.setComponentsDestroyFunction(
                    [ecs, hostId, hasVelocity, hasAction](Rengine::Entity& en) {
                        std::optional<std::reference_wrapper<Relationship>> hostRelationship = std::nullopt;

                        try {
                            Rengine::Entity& host = ecs->getEntity(hostId);
                            hostRelationship = host.getComponentNoExcept<Relationship>();
                        } catch (Rengine::ECSExceptionEntityNotFound& e) {
                            std::cerr << "[projectileLambda] Error: " << e.what() << std::endl;
                        }
                        // Remove child projectile on destruction
                        if (hostRelationship.has_value() == true) {
                            hostRelationship.value().get().removeChild(uint64_t(en));
                        }
                        en.removeComponent<Chrono>();
                        if (hasAction) {
                            en.removeComponent<Action>();
                        }
                        if (hasVelocity) {
                            en.removeComponent<Velocity>();
                        }
                    }
                );
            } // for it
        }

        inline void Action::handleShootBuff(Network::EntityAction& action, std::shared_ptr<Rengine::ECS>& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            std::cout << "Buff not inplemented yet. Update Components::Action" << std::endl;
        }

    }  // namespace Components
}  // namespace RType
