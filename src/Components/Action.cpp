//
#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Graphics/Vector.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

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

namespace RType {
    namespace Components {

        Action::Action(std::reference_wrapper<SceneManager> sceneManager, ActionSource source, const std::string& scriptPath)
            : _sceneManager(sceneManager)
        {
            this->_actionSource = source;
            switch (source) {
                case ActionSourceScript:
                    std::cout << "RType::Component::Action: Warning: Script not inplemented." << scriptPath << std::endl;
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
            switch (this->_actionSource) {
                // scripts WIP
                case ActionSource::ActionSourceScript:
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

        void Action::processUserInput(void)
        {
            // Check if source is UserInput
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on non user source.");
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
                throw ActionException("Trying to set user input on non user source.");
            }
            Network::EntityAction newAction;

            // Parse input vector
            auto it = PlayerInputBindVector.begin();

            while (it != PlayerInputBindVector.end()) {
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
            if (it == PlayerInputBindVector.end()) {
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
            // Check if source is lua script
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Trying to set user input on lua source.");
            }
        }
        const Rengine::Graphics::UserInput Action::getPlayerNeededInput(Network::EntityActionType act) const
        {
            if (this->_actionSource != ActionSourceUserInput) {
                throw ActionException("Accessing player input on non user source.");
            }
            auto it = PlayerInputBindVector.begin();

            while (it != PlayerInputBindVector.end()) {
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
                    component._shootDeltatimes[i] += Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getDeltaTimeSeconds();
            }
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
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            // Not enough entity left for attack : skip it
            if (attackConfig->getMissiles().size() > ecs.getEntityLimit() - ecs.getActiveEntitiesCount()) {
                return;
            }
            Config::EntityConfigResolver singletone = Config::EntityConfigResolverSingletone::get();
            const Position& hostPosition = entity.getComponent<Position>();

            for (const Config::MissileConfig& it : attackConfig->getMissiles()) {
                const RType::Config::EntityConfig& missileConfig = singletone.get(it.getJsonPath());
                Rengine::Entity& projectile = ecs.addEntity();
                RType::Components::Relationship& relationship = projectile.addComponent<RType::Components::Relationship>();

                // set Components
                projectile.addComponent<HitboxViewer>(missileConfig.getHitbox().size.x, missileConfig.getHitbox().size.y);
                projectile.addComponent<Position>(
                        hostPosition.getVector2D().x + it.getOffset().first,
                        hostPosition.getVector2D().y + it.getOffset().second
                );
                projectile.addComponent<Hitbox>(missileConfig.getHitbox());
                projectile.addComponent<HitboxViewer>(missileConfig.getHitbox().size.x, missileConfig.getHitbox().size.y);
                projectile.addComponent<Sprite>(missileConfig.getSprite().getSpecs());
                projectile.addComponent<Configuration>(missileConfig);
                projectile.addComponent<Buff>();

                projectile.addComponent<Chrono>([&ecs, &projectile]() {
                    ecs.removeEntity(projectile);
                }, 7.0f);

                relationship.addParent(uint64_t(entity));

                projectile.setComponentsDestroyFunction(
                    [](Rengine::Entity& en) {
                        en.removeComponent<HitboxViewer>();
                        en.removeComponent<Position>();
                        en.removeComponent<Hitbox>();
                        en.removeComponent<HitboxViewer>();
                        en.removeComponent<Sprite>();
                        en.removeComponent<Configuration>();
                        en.removeComponent<Buff>();
                        en.removeComponent<Relationship>();
                        en.removeComponent<Chrono>();
                    }
                );

                switch (it.getControlType()) {
                    case (Config::MissileControlTypeUserInput):
                        projectile.addComponent<Action>(actionComponent._sceneManager, ActionSource::ActionSourceUserInput);
                        break;

                    case (Config::MissileControlTypeScript):
                        projectile.addComponent<Action>(actionComponent._sceneManager, ActionSource::ActionSourceScript, it.getScriptPath());
                        break;

                    case (Config::MissileControlTypeVelocity):
                        projectile.addComponent<Velocity>(it.getVelocity().first, it.getVelocity().second);
                        break;

                    // No action component needed for invalid value
                    default:
                        break;
                } // switch controlType
                actionComponent._sceneManager.get().addEntityToCurrentScene(Rengine::Entity::size_type(projectile));
            } // for it
        }

        inline void Action::handleShootBuff(Network::EntityAction& action, Rengine::ECS& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig)
        {
            std::cout << "Buff not inplemented yet. Update Components::Action" << std::endl;
        }

    }  // namespace Components
}  // namespace RType
