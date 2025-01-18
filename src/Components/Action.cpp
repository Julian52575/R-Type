//
#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <rengine/src/Clock/Clock.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/Vector.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "src/Game/EntityMaker.hpp"
#include "../Network/EntityAction.hpp"
#include "Components.hpp"
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
#include "src/Components/HitboxViewer.hpp"
#include "src/Components/Velocity.hpp"
#include "src/Components/Chrono.hpp"
#include "src/Components/Life.hpp"
#include "src/Game/Team.hpp"

#include "src/Game/LuaManager.hpp"

namespace RType {
    namespace Components {

        Action::Action(ActionSource source, const std::string& scriptPath)
        {
            this->_actionSource = source;
            switch (source) {
                case ActionSourceScript:
                    this->_luaInfos.id = RType::LuaManagerSingletone::get().loadLuaScript(scriptPath);
                    this->_luaInfos.scriptPath = scriptPath;

                    std::cout << "Script loaded: " << scriptPath << " with id: " << this->_luaInfos.id << std::endl;
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

        void Action::updateFromSource(void) noexcept
        {
            std::vector<RType::LuaReturn> reply;

            switch (this->_actionSource) {
                // scripts WIP
                case ActionSource::ActionSourceScript:
                    reply = RType::LuaManagerSingletone::get().callFunction(this->_luaInfos.scriptPath, this->_luaInfos.id, "movement", 0,0,0,0);
                    reply = RType::LuaManagerSingletone::get().callFunction(this->_luaInfos.scriptPath, this->_luaInfos.id, "attack", true,0,0);
                    return;

                case ActionSource::ActionSourceUserInput:
                    this->processUserInput();
                    break;

                case ActionSource::ActionSourceServer:
                    return;

                default:
                    return;
            }
        }

        std::vector<Network::EntityAction> &Action::getActionVector(void) noexcept
        {
            return this->_actionVector;
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
                if (it->type == Rengine::Graphics::UserInputTypeKeyboardCharPressed || it->type == Rengine::Graphics::UserInputTypeKeyboardCharPressed
                || it->type == Rengine::Graphics::UserInputTypeKeyboardSpecialPressed || it->type == Rengine::Graphics::UserInputTypeKeyboardSpecialPressed) {
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

        ActionSource Action::getActionSource(void) const noexcept
        {
            return this->_actionSource;
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
                    if (it->first.type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
                        dataComparaison = (it->first.data.keyboardSpecial == input.data.keyboardSpecial);
                    } else if (it->first.type == Rengine::Graphics::UserInputTypeKeyboardChar) {  // Check KeyboardChar
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
                if (input.type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
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
                // set minimum speed for smoother movement
                if (newAction.data.moveVelocity.x != 0 && newAction.data.moveVelocity.x < 20.0f) {
                    newAction.data.moveVelocity.x = 50.0f;
                }
            }
            this->_actionVector.push_back(newAction);
        }

        void Action::processAction(const Network::EntityAction& rfcAction) noexcept
        {
            this->_actionVector.push_back(rfcAction);
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

            if (entityConfig.has_value() == false || pos.has_value() == false) {
                return;
            }
            actionComponent.updateFromSource();
            const Rengine::Graphics::vector2D<float>& currentPos = pos->get().getVector2D();
            Rengine::Graphics::vector2D<float> newPos = currentPos;

            for (auto it : actionComponent) {
                // Move
                if (it.type == Network::EntityActionTypeMove) {
                    actionComponent.handleMove(it, entityConfig.value(), pos.value());
                }
                // Shoot1 -> 3
                if (Network::EntityActionTypeShoot1 <= it.type && it.type <= Network::EntityActionTypeShoot3) {
                    actionComponent.handleShoot(actionComponent, it, ecs, entity, entityConfig.value());
                }
            }  // for it
            actionComponent.clear();
        }

        void updateDeltatimes(Action& component) noexcept
        {
            for (uint8_t i = 0; i < 3; i++) {
                if (component._shootDeltatimes[i] < 10000.0f)
                    component._shootDeltatimes[i] += Rengine::Clock::getElapsedTime();
            }
        }

        void Action::handleMove(Network::EntityAction& action, Configuration& config, Position& pos)
        {
            float deltatime = Rengine::Clock::getElapsedTime();
            Rengine::Graphics::vector2D<float> newPos = pos.getVector2D();

            newPos.x += action.data.moveVelocity.x * (config.getConfig().getStats().speedX * deltatime);
            newPos.y += action.data.moveVelocity.y * (config.getConfig().getStats().speedY * deltatime);
            pos.set(newPos);
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
                projectile.addComponent<HitboxViewer>(currentMissileEntityConfig.getHitbox().size.x, currentMissileEntityConfig.getHitbox().size.y);
                projectile.addComponent<Chrono>([&ecs, &projectile]() {
                    ecs.removeEntity(projectile);
                }, 7.0f);
                hostRelationship.addChild(uint64_t(projectile));
                proRelationship.addParent(uint64_t(host));
                bool hasAction = false;
                bool hasVelocity = false;
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
                        projectile.addComponent<Velocity>(it.getVelocity().first, it.getVelocity().second);
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
                        en.removeComponent<HitboxViewer>();
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
