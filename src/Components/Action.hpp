//
#ifndef SRC_COMPONENTS_ACTION_HPP_
#define SRC_COMPONENTS_ACTION_HPP_
#include <cstddef>
#include <functional>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/Vector.hpp>
#include <stdexcept>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <rengine/Rengine.hpp>

#include "../Network/EntityAction.hpp"
#include "src/Components/Velocity.hpp"
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
                void updateFromSource(Rengine::ECS& ecs, Rengine::Entity& entity) noexcept;
                /**
                * @fn processAction
                * @brief Process an input from the received paquet.
                */
                void processAction(const Network::EntityAction& rfcAction) noexcept;

                /*      Function for ECS        */
                /**
                * @fn componentFunction
                * @param ECS A reference to the ecs in use.
                * @param component A reference to an Action component
                * @param entity A reference to the entity who own the component.
                * @brief Process the list of Network::Action registered in this component.
                */
                static void componentFunction(Rengine::ECS& ecs, Action& component, Rengine::Entity& entity, std::shared_ptr<Rengine::ECS> &sharedEcs);
                friend void updateDeltatimes(Action& component) noexcept;
                inline void handleMove(Network::EntityAction& action, RType::Components::Configuration& config, RType::Components::Velocity& vel);
                inline void handleShoot(Action& actionComponent, Network::EntityAction& action,
                        std::shared_ptr<Rengine::ECS>& ecs, Rengine::Entity& entity, Configuration& entityConfig);
                friend inline void handleShootMissile(Action& actionComponent, Network::EntityAction& action, std::shared_ptr<Rengine::ECS>& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig);
                inline void handleShootBuff(Network::EntityAction& action, std::shared_ptr<Rengine::ECS>& ecs,
                        Rengine::Entity& entity, Configuration& entityConfig, const std::optional<Config::AttackConfig>& attackConfig);

                container_t &getActionVector(void) noexcept;
                ActionSource getActionSource(void) const noexcept;

            private:
                /**
                 * @brief Call lua scripts and add output in the vector
                */
                void processLuaScriptInput(Rengine::ECS& ecs, Rengine::Entity& entity);

            private:
                ActionSource _actionSource;
                std::optional<std::string> _scriptPath;
                container_t _actionVector;
                float _shootDeltatimes[3] = {0.0f};
                struct LuaInfos _luaInfos;
                Rengine::Graphics::vector2D<bool> _updatedNonZeroVelocity = {false, false};
        };  // class Action
    }  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_ACTION_HPP_
