//
#ifndef SRC_COMPONENTS_ACTION_HPP_
#define SRC_COMPONENTS_ACTION_HPP_
#include <cstddef>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <stdexcept>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "../Network/EntityAction.hpp"

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
            ActionSourceLua,
            ActionSourceUserInput
        };

        class Action {
            public:
                using container_t = std::vector<Network::EntityAction>;
                using size_type = typename container_t :: size_type;
                using iterator = typename container_t :: iterator;
                using const_iterator = typename container_t :: const_iterator;
                explicit Action(ActionSource source, const std::string& luaScript = "");
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
                * @fn addUserInput
                * @param input The user input to process.
                * @exception ActionException when ActionSource != ActionSourceUserInput
                * @brief Process the input and add a new EntityAction inside the vector.
                */
                void processUserInput(const Rengine::Graphics::UserInput& input);
                /**
                * @fn changeInput
                * @param resultingAction The EntityActionType to happen when newInput is processed.
                * @param outcome newInput The new input to trigger resultingAction
                * @exception ActionException when ActionSource != ActionSourceUserInput
                * @brief Change the needed input for the outcome action.
                */
                void changeInput(Rengine::Graphics::UserInput newInput, Network::EntityActionType resultingAction);
                /**
                * @fn getNeededInput
                * @param outcome The EntityActionType to analyse.
                * @return The UserInput needed to trigger the outcome.
                * UserInput.type == UserInputTypeNA when no match was found.
                * @brief Get the UserInput needed to trigger an EntityActionType
                */
                const Rengine::Graphics::UserInput getNeededInput(Network::EntityActionType outcome) const;

            private:
                ActionSource _actionSource;
                container_t _actionVector;
                void buildBindVector(void);
                std::vector<std::pair<Rengine::Graphics::UserInput, Network::EntityActionType>> _inputNetworkBindVector;
        };  // class Action
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_ACTION_HPP_
