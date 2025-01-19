//
#include <chrono>
#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <rengine/src/Clock/Clock.hpp>
#include <rengine/src/Entity.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "src/Game/LuaManager.hpp"
#include "src/Game/EntityMaker.hpp"
#include "src/Network/EntityAction.hpp"
#include "src/Game/SceneManager.hpp"
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
#include "src/Components/Sprite.hpp"
#ifdef DEBUG
    #include "src/Components/HitboxViewer.hpp"
    #include "src/Components/HealthViewer.hpp"
#endif
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
                    this->_luaInfos.id = RType::LuaManagerSingletone::get().loadLuaScript(scriptPath);
                    this->_luaInfos.scriptPath = scriptPath;
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
                // scripts WIP
                case ActionSource::ActionSourceScript:
                    try {
                        this->processLuaScriptInput(ecs, entity);
                    } catch (...) {
                        std::cerr << "Script " << this->_luaInfos.scriptPath << " received execption: " << ". Disabling it..." << std::endl;
                        this->_actionSource = ActionSource::ActionSourceNA;
                        return;
                    }
                    break;

                case ActionSource::ActionSourceUserInput:
                    this->processUserInput();
                    break;

                case ActionSource::ActionSourceServer:
                    return;

                default:
                    return;
            }
        }

        void Action::processUserInput(void)
        {
            // Check if source is UserInput
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on non user source.");
            }
            Rengine::Graphics::UserInputManager& inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
            Rengine::Graphics::UserInputManager::const_iterator it = inputManager.begin();

            while (it != inputManager.end()) {
                // Check keyboard
                if (it->type == Rengine::Graphics::UserInputTypeKeyboardCharPressed || it->type == Rengine::Graphics::UserInputTypeKeyboardChar
                || it->type == Rengine::Graphics::UserInputTypeKeyboardSpecialPressed || it->type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
                    this->processUserInputKeyboard(*it);
                } else if (it->type == Rengine::Graphics::UserInputTypeJoystickButton
                || it->type == Rengine::Graphics::UserInputTypeJoystickLeftMove || it->type == Rengine::Graphics::UserInputTypeJoystickLeftPressed
                || it->type == Rengine::Graphics::UserInputTypeJoystickRightMove || it->type == Rengine::Graphics::UserInputTypeJoystickRightPressed) {
                    // Check joystick
                    this->processUserInputJoystick(*it);
                }
                it++;
            }
        }
        void Action::processUserInputKeyboard(const Rengine::Graphics::UserInput& input)
        {
            // Check if source is UserInput
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on non user source.");
            }
            Network::EntityAction newAction;

            // Parse input vector
            auto it = PlayerKeyboardInputBindVector.begin();

            while (it != PlayerKeyboardInputBindVector.end()) {
                bool dataComparaison = false;

                if (it->first.type == input.type) {
                    // Check KeyboardSpecial
                    if (it->first.type == Rengine::Graphics::UserInputTypeKeyboardSpecial
                    || it->first.type == Rengine::Graphics::UserInputTypeKeyboardSpecialPressed) {
                        dataComparaison = (it->first.data.keyboardSpecial == input.data.keyboardSpecial);
                    } else if (it->first.type == Rengine::Graphics::UserInputTypeKeyboardChar
                    || it->first.type == Rengine::Graphics::UserInputTypeKeyboardCharPressed) {  // Check KeyboardChar
                        dataComparaison = (it->first.data.keyboardChar == input.data.keyboardChar);
                    }
                    if (dataComparaison == true) {
                        newAction.type = it->second;
                        break;
                    }
                }
                it++;
            }  // while it != end
            // No match
            if (it == PlayerKeyboardInputBindVector.end()) {
                return;
            }

            // Handle movement
            if (newAction.type == Network::EntityActionTypeMove) {
                float moveX = 0.0f;
                float moveY = 0.0f;
                // Handle keyboard arrows
                if (input.type == Rengine::Graphics::UserInputTypeKeyboardSpecial || input.type == Rengine::Graphics::UserInputTypeKeyboardSpecialPressed) {
                    switch (input.data.keyboardSpecial) {
                        // Down arrow
                        case Rengine::Graphics::UserInputKeyboardSpecialArrowDOWN:
                            moveY = 80;
                            break;
                        // Up arrow
                        case Rengine::Graphics::UserInputKeyboardSpecialArrowUP:
                            moveY = -80;
                            break;
                        // Left arrow
                        case Rengine::Graphics::UserInputKeyboardSpecialArrowLEFT:
                            moveX = -80;
                            break;
                        // Right arrow
                        case Rengine::Graphics::UserInputKeyboardSpecialArrowRIGHT:
                            moveX = 80;
                            break;
                        // Error handling
                        default:
                            break;
                    }  // switch input.data.keyboardSpecial
                }
                newAction.data.moveVelocity.x = moveX;
                newAction.data.moveVelocity.y = moveY;
            }  // if newAction.type == Network::EntityActionTypeMove
            this->_actionVector.push_back(newAction);
        }
        void Action::processUserInputJoystick(const Rengine::Graphics::UserInput& input)
        {
            // Check if source is UserInput
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on non user source.");
            }
            Network::EntityAction newAction;

            // Parse input vector
            auto it = PlayerJoystickInputBindVector.begin();

            while (it != PlayerJoystickInputBindVector.end()) {
                bool dataComparaison = false;

                if (it->first.type == input.type) {
                    if (it->first.type == Rengine::Graphics::UserInputTypeJoystickButton) {
                        if (it->first.data.joystickInput.data.joystickButton == input.data.joystickInput.data.joystickButton) {
                        newAction.type = it->second;
                        break;
                        }
                    } else {
                      // No other comparison needed for other types
                        newAction.type = it->second;
                        break;
                    }
                }
                it++;
            }  // while it != end
            // No match
            if (it == PlayerJoystickInputBindVector.end()) {
                return;
            }

            // Handle movement
            if (newAction.type == Network::EntityActionTypeMove) {
                newAction.data.moveVelocity.x = input.data.joystickInput.data.joystickPosition.x;
                newAction.data.moveVelocity.y = input.data.joystickInput.data.joystickPosition.y;
            }
            this->_actionVector.push_back(newAction);
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

            std::cout << "[processLuaScriptInput]" << std::endl;  //
            // no pos ? no lua
            if (pos.has_value() == false) {
                return;
            }
            std::cout << "[processLuaScriptInput] before deltatime" << std::endl;
            // update deltatime
            RType::LuaManagerSingletone::get().callFunction(this->_luaInfos.scriptPath, this->_luaInfos.id, "updateDeltatime", Rengine::Clock::getElapsedTime());

            std::cout << "[processLuaScriptInput] after deltatime" << std::endl;
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
            std::cout << "[processLuaScriptInput] Move :" << act << std::endl;
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
            std::cout << "[processLuaScriptInput] Shoot :" << act << std::endl;
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
        void Action::changePlayerInput(Rengine::Graphics::UserInput newInput, Network::EntityActionType resultingAction)
        {
            auto it = PlayerKeyboardInputBindVector.begin();

            while (it != PlayerKeyboardInputBindVector.end()) {
                if (it->second == resultingAction) {
                    it->first = newInput;
                    return;
                }
                it++;
            }
            auto jIt = PlayerJoystickInputBindVector.begin();

            while (it != PlayerJoystickInputBindVector.end()) {
                if (it->second == resultingAction) {
                    it->first = newInput;
                    return;
                }
                it++;
            }
        }
        const Rengine::Graphics::UserInput Action::getPlayerNeededInput(Network::EntityActionType act)
        {
            auto it = PlayerKeyboardInputBindVector.begin();

            while (it != PlayerKeyboardInputBindVector.end()) {
                if (it->second == act) {
                    return it->first;
                }
                it++;
            }
            auto jIt = PlayerJoystickInputBindVector.begin();

            while (it != PlayerJoystickInputBindVector.end()) {
                if (it->second == act) {
                    return it->first;
                }
                it++;
            }
            return {Rengine::Graphics::UserInputTypeNA, {0}};
        }

        void Action::componentFunction(Rengine::ECS& ecs, Action& actionComponent, Rengine::Entity& entity)
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

            std::cout << "[componentFunction] Entity --" << entity << std::endl;  ///
            for (auto it : actionComponent) {
                // Move
                std::cout << "[componentFunction] it = " << it << std::endl;  ///
                if (it.type == Network::EntityActionTypeMove && vel.has_value() == true) {
                    actionComponent.handleMove(it, entityConfig.value(), vel->get());
                }
                // Shoot1 -> 3
                if (Network::EntityActionTypeShoot1 <= it.type && it.type <= Network::EntityActionTypeShoot3) {
                    actionComponent.handleShoot(actionComponent, it, ecs, entity, entityConfig.value());
                }
            }  // for it
            std::cout << "[componentFunction] Entity --" << entity << std::endl;  ///
            actionComponent.clear();
            // No move input : no velocity
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

        void Action::handleMove(Network::EntityAction& action, Configuration& config, RType::Components::Velocity& vel)
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

        void Action::handleShoot(Action& actionComponent, Network::EntityAction& action, Rengine::ECS& ecs, Rengine::Entity& entity, Configuration& entityConfig)
        {
            // Get attackId by taking advantage of the fact that the 3 TypeShoot follow each other in the enum
            uint8_t attackId = 1 + (action.type - Network::EntityActionTypeShoot1);
            const std::optional<Config::AttackConfig>& attackConfig = entityConfig.getConfig().getAttack(attackId);

            // No attack : no action
            if (attackConfig.has_value() == false) {
                return;
            }
            std::cout << "[handleShoot] Entity " << entity << " requested " << action << std::endl;
            // Cooldown not reached : no shoot
            if (this->_shootDeltatimes[attackId - 1] < attackConfig->getCooldown()) {
                return;
            }
            std::cout << "[handleShoot] deltatime OK" << std::endl;
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

        inline void handleShootMissile(Action& actionComponent, Network::EntityAction& action, Rengine::ECS& ecs,
                        Rengine::Entity& host, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            // Not enough entity left for attack : skip it
            if (attackConfig->getMissiles().size() > ecs.getEntityLimit() - ecs.getActiveEntitiesCount()) {
                return;
            }
            Config::EntityConfigResolver singletone = Config::EntityConfigResolverSingletone::get();
            const Position& hostPosition = host.getComponent<Position>();
            Relationship& hostRelationship = host.getComponent<Relationship>();

            for (const Config::MissileConfig& it : attackConfig->getMissiles()) {
                RType::Config::EntityConfig currentMissileEntityConfig;
                Rengine::Entity& projectile = RType::EntityMaker::make(ecs, it.getJsonPath(), hostRelationship.getGroup(), &currentMissileEntityConfig);
                RType::Components::Relationship& proRelationship = projectile.getComponent<RType::Components::Relationship>();

                // set Components
                projectile.getComponent<Position>().set(
                        {hostPosition.getVector2D().x + (float) it.getOffset().first,
                        hostPosition.getVector2D().y + (float) it.getOffset().second}
                );
                projectile.addComponent<Sprite>(currentMissileEntityConfig.getSprite().getSpecs());
                if (hostRelationship.belong(Team::TeamEnemy) == true) {
                    projectile.getComponent<Sprite>().getSprite()->flip();
                }

            #ifdef DEBUG
                projectile.addComponent<HitboxViewer>(currentMissileEntityConfig.getHitbox().size.x, currentMissileEntityConfig.getHitbox().size.y);
                //projectile.addComponent<HealthViewer>(currentMissileEntityConfig.getStats().hp);
            #endif
                projectile.addComponent<Chrono>([&ecs, &projectile]() {
                    ecs.removeEntity(projectile);
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
                    [&host, hasVelocity, hasAction](Rengine::Entity& en) {
                        auto hostRelationship = host.getComponentNoExcept<Relationship>();

                        // Remove child projectile on destruction
                        if (hostRelationship.has_value() == true) {
                            hostRelationship.value().get().removeChild(uint64_t(en));
                        }
                        en.removeComponent<Sprite>();
                #ifdef DEBUG
                        en.removeComponent<HitboxViewer>();
                        //en.removeComponent<HealthViewer>();
                #endif
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

        inline void Action::handleShootBuff(Network::EntityAction& action, Rengine::ECS& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            std::cout << "Buff not inplemented yet. Update Components::Action" << std::endl;
        }

    }  // namespace Components
}  // namespace RType
