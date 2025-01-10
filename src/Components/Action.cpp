//
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/Vector.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "../Network/EntityAction.hpp"
#include "Action.hpp"
#include "Configuration.hpp"
#include "Position.hpp"
#include "Sprite.hpp"
#include "src/Config/AttackBuffTypeEnum.hpp"
#include "src/Config/AttackConfig.hpp"
#include "src/Config/EntityConfig.hpp"
#include "src/Config/EntityConfigResolver.hpp"

namespace RType {
    namespace Components {

        Action::Action(std::reference_wrapper<SceneManager> sceneManager, ActionSource source, const std::string& luaScript)
            : _sceneManager(sceneManager)
        {
            this->_actionSource = source;
            switch (source) {
                case ActionSourceLua:
                    std::cout << "RType::Component::Action: Warning: Lua not inplemented." << luaScript << std::endl;
                    break;

                case ActionSourceUserInput:
                    this->buildBindVector();
                    break;

                default:
                    throw std::runtime_error("RType::Component::Action: Unknow ActionSource.");
            }  // switch source
            this->_actionVector.reserve(5);
        }
        void Action::processUserInput(void)
        {
            // Check if source is UserInput
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on lua source.");
            }
            Rengine::Graphics::UserInputManager& inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
            Rengine::Graphics::UserInputManager::const_iterator it = inputManager.begin();

            while (it != inputManager.end()) {
                this->processUserInput(*it);
                it++;
            }
        }
        void Action::processUserInput(const Rengine::Graphics::UserInput& input)
        {
            // Check if source is UserInput
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on lua source.");
            }
            Network::EntityAction newAction;

            // Parse input vector
            auto it = this->_inputNetworkBindVector.begin();

            while (it != this->_inputNetworkBindVector.end()) {
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
            if (it == this->_inputNetworkBindVector.end()) {
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
                } else if (input.type == Rengine::Graphics::UserInputTypeJoystickLeftMove
                        || input.type == Rengine::Graphics::UserInputTypeJoystickRightMove) {
                    moveX = input.data.joystickPosition.x;
                    moveY = input.data.joystickPosition.y;
                }
                newAction.data.moveVelocity.x = moveX;
                newAction.data.moveVelocity.y = moveY;
            }  // if newAction.type == Network::EntityActionTypeMove
            this->_actionVector.push_back(newAction);
        }

        void Action::buildBindVector(void)
        {
            this->_inputNetworkBindVector = {
                {
                    {Rengine::Graphics::UserInputTypeKeyboardChar, ' '},
                    Network::EntityActionType::EntityActionTypeShoot1
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialSHIFT},
                    Network::EntityActionType::EntityActionTypeShoot2
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialTAB},
                    Network::EntityActionType::EntityActionTypeShoot3
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialArrowUP},
                    Network::EntityActionType::EntityActionTypeMove
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialArrowDOWN},
                    Network::EntityActionType::EntityActionTypeMove
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialArrowLEFT},
                    Network::EntityActionType::EntityActionTypeMove
                },
                {
                    {Rengine::Graphics::UserInputTypeKeyboardSpecial, Rengine::Graphics::UserInputKeyboardSpecialArrowRIGHT},
                    Network::EntityActionType::EntityActionTypeMove
                }
            };  // this->_inputNetworkBindVector
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
        void Action::changeInput(Rengine::Graphics::UserInput newInput, Network::EntityActionType resultingAction)
        {
            // Check if source is lua script
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on lua source.");
            }
        }
        const Rengine::Graphics::UserInput Action::getNeededInput(Network::EntityActionType act) const
        {
            auto it = this->_inputNetworkBindVector.begin();

            while (it != this->_inputNetworkBindVector.end()) {
                if (it->second == act) {
                    return it->first;
                }
                it++;
            }
            return {Rengine::Graphics::UserInputTypeNA, {0}};
        }

        void Action::componentFunction(Rengine::ECS& ecs, Action& actionComponent, Rengine::Entity& entity)
        {
            std::optional<std::reference_wrapper<Configuration>> entityConfig = entity.getComponentNoExcept<Configuration>();
            std::optional<std::reference_wrapper<Position>> pos = entity.getComponentNoExcept<Position>();

            if (entityConfig.has_value() == false || pos.has_value() == false) {
                return;
            }
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

        void Action::handleMove(Network::EntityAction& action, Configuration& config, Position& pos)
        {
            float deltatime = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getDeltaTimeSeconds();
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

            if (attackConfig.has_value() == false) {
                return;
            }
            std::cout << "Shoot " << attackId << std::endl; // debug
                                                        #warning Debug print in Action.cpp
            switch (attackConfig->getType()) {
                // Handle buffs
                case (Config::AttackType::AttackTypeBuffs):
                    actionComponent.handleShootBuff(action, ecs, entity, entityConfig, attackConfig);
                    break;
                // Handle missiles
                case (Config::AttackType::AttackTypeMissiles):
                    actionComponent.handleShootMissile(action, ecs, entity, entityConfig, attackConfig);
                    break;
                default:
                    return;
            }
        }

        inline void Action::handleShootMissile(Network::EntityAction& action, Rengine::ECS& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            // No projectile when no more entity left
            if (ecs.getActiveEntitiesCount() >= ecs.getEntityLimit()) {
                return;
            }
            RType::Config::EntityConfigResolver& resolver = RType::Config::EntityConfigResolverSingletone::get();
            RType::Config::EntityConfig missileConfig("assets/entities/missile1.json");
            Rengine::Entity& projectile = ecs.addEntity();

            projectile.addComponent<RType::Components::Configuration>(missileConfig);
            projectile.addComponent<RType::Components::Sprite>(missileConfig.getSprite().getSpecs());
            projectile.addComponent<RType::Components::Position>(0, 0);
            this->_sceneManager.get().addEntityToCurrentScene(projectile);
        }

        inline void Action::handleShootBuff(Network::EntityAction& action, Rengine::ECS& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            std::cout << "Buff not inplemented yet. Update Components::Action" << std::endl;
        }

    }  // namespace Components
}  // namespace RType
