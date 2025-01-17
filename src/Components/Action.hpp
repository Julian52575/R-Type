//
#ifndef SRC_COMPONENTS_ACTION_HPP_
#define SRC_COMPONENTS_ACTION_HPP_
#include <cstddef>
#include <functional>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <stdexcept>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "../Network/EntityAction.hpp"
#include "../Game/SceneManager.hpp"
#include "Position.hpp"
#include "Configuration.hpp"
#include "src/Config/AttackConfig.hpp"

namespace RType {

    namespace Components {

        class ActionException : public std::exception {
            public:
                explicit ActionException(const std::string& msg)
                {
                    this->_msg = "RType::Component::Action: " + msg;
                }
                ~ActionException(void) = default;
                const char *what() const noexcept {
                    return this->_msg.c_str();
                }

            private:
                std::string _msg;
        };

        enum ActionSource {
            ActionSourceNA,
            ActionSourceScript,
            ActionSourceUserInput,
            ActionSourceServer
        };

        /**
        * @brief The player input and the equivalent EntityAction
        */
        static std::vector<std::pair<Rengine::Graphics::UserInput, Network::EntityActionType>> PlayerKeyboardInputBindVector = {
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
        };
        static std::vector<std::pair<Rengine::Graphics::UserInput, Network::EntityActionType>> PlayerJoystickInputBindVector = {
            {
                 {.type= Rengine::Graphics::UserInputTypeJoystickButton, .data= {.joystickInput= {.data= {.joystickButton= 0}}}},
                 Network::EntityActionType::EntityActionTypeShoot1
            },
            {
                 {.type= Rengine::Graphics::UserInputTypeJoystickButton, .data= {.joystickInput= {.data= {.joystickButton= 4}}}},
                 Network::EntityActionType::EntityActionTypeShoot2
            },
            {
                 {.type= Rengine::Graphics::UserInputTypeJoystickButton, .data= {.joystickInput= {.data= {.joystickButton= 5}}}},
                 Network::EntityActionType::EntityActionTypeShoot3
            },
            {
                 {.type= Rengine::Graphics::UserInputTypeJoystickLeftMove},
                 Network::EntityActionType::EntityActionTypeMove
            }
        };


        struct LuaInfos {
            std::string scriptPath;
            int id;
        };

        class Action {
            public:
                using container_t = std::vector<Network::EntityAction>;
                using size_type = typename container_t :: size_type;
                using iterator = typename container_t :: iterator;
                using const_iterator = typename container_t :: const_iterator;
                /**
                * @fn Action
                * @param entitySceneManager The scene manager of the scene the entity belong to
                * @param source The source of the action
                * @param scriptPath The path to the script to execute if ActionSource == ActionSourceLua
                * @brief Create an instance of Action Component.
                */
                Action(ActionSource source, const std::string& scriptPath = "");
                ~Action(void) = default;
                /**
                * @fn begin
                * @return A const iterator of Network::EntityAction.
                * @brief Get a const iterator from the Network::EntityAction vector.
                */
                const_iterator begin(void) const;
                const_iterator end(void) const;
                /**
                * @fn clear
                * @brief Reset the action vector by removing all the EntityAction.
                */
                void clear(void);
                /**
                * @fn size
                * @return The number of EntityAction in the vector.
                * @brief Get the number of EntityAction.
                */
                size_type size(void) const noexcept;
                /**
                * @fn processInput
                * @brief Process the inputs from the ActionSource
                */
                void updateFromSource(void) noexcept;
                /**
                * @fn processAction
                * @brief Process an input from the received paquet.
                */
                void processAction(const Network::EntityAction& rfcAction) noexcept;

                /**
                * @fn changeInput
                * @param resultingAction The EntityActionType to happen when newInput is processed.
                * @param outcome newInput The new input to trigger resultingAction
                * @exception ActionException when ActionSource != ActionSourceUserInput
                * @brief Change the needed input for the outcome action.
                */
                static void changePlayerInput(Rengine::Graphics::UserInput newInput, Network::EntityActionType resultingAction);
                /**
                * @fn getNeededInput
                * @param outcome The EntityActionType to analyse.
                * @return The UserInput needed to trigger the outcome.
                * UserInput.type == UserInputTypeNA when no match was found.
                * @brief Get the UserInput needed to trigger an EntityActionType
                */
                static const Rengine::Graphics::UserInput getPlayerNeededInput(Network::EntityActionType outcome);

                /*      Function for ECS        */
                /**
                * @fn componentFunction
                * @param ECS A reference to the ecs in use.
                * @param component A reference to an Action component
                * @param entity A reference to the entity who own the component.
                * @brief Process the list of Network::Action registered in this component.
                */
                static void componentFunction(Rengine::ECS& ecs, Action& component, Rengine::Entity& entity);
                friend void updateDeltatimes(Action& component) noexcept;
                inline void handleMove(Network::EntityAction& action, RType::Components::Configuration& config, RType::Components::Position& pos);
                inline void handleShoot(Action& actionComponent, Network::EntityAction& action,
                        Rengine::ECS& ecs, Rengine::Entity& entity, Configuration& entityConfig);
                friend inline void handleShootMissile(Action& actionComponent, Network::EntityAction& action, Rengine::ECS& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig);
                inline void handleShootBuff(Network::EntityAction& action, Rengine::ECS& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig);

            private:
                /**
                * @fn addUserInput
                * @exception ActionException when ActionSource != ActionSourceUserInput
                * @brief Parse all input from the UserInputManager and add new EntityActions inside the vector.
                */
                void processUserInput(void);
                /**
                * @fn addUserInput
                * @param input The user input to process.
                * @exception ActionException when ActionSource != ActionSourceUserInput
                * @brief Process the input and add a new EntityAction inside the vector.
                */
                void processUserInputKeyboard(const Rengine::Graphics::UserInput& input);
                /**
                * @brief Same for joysticks
                */
                void processUserInputJoystick(const Rengine::Graphics::UserInput& input);

            private:
                ActionSource _actionSource;
                std::optional<std::string> _scriptPath;
                container_t _actionVector;
                float _shootDeltatimes[3] = {0.0f};
                struct LuaInfos _luaInfos;
        };  // class Action
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_ACTION_HPP_
