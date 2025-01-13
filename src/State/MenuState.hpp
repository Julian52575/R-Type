//
#ifndef SRC_STATE_MENUSTATE_HPP_
#define SRC_STATE_MENUSTATE_HPP_

#include <rengine/Rengine.hpp>

#include "AState.hpp"
#include "State.hpp"
#include "LobbyState.hpp"


#include "src/Config/LevelConfigResolver.hpp"
#include "src/Components/Buff.hpp"
#include "src/Components/Sprite.hpp"
#include "src/Components/Position.hpp"
#include "src/Components/Action.hpp"
#include "src/Components/Components.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Relationship.hpp"

namespace RType {

    enum RTypeMenuScenes {
        // Placeholder enum
        RTypeMenuScenesNA,
        RTypeMenuScenesAttractMode,
        RTypeMenuScenesSelectLobby,
        RTypeMenuScenesButtonDisplay,
    };

    class MenuState : public AState {
        public:
            MenuState(Rengine::ECS& ecs) : AState(ecs) {
                this->registerComponents();
                this->makeButton();
                this->_currentScene = RTypeMenuScenes::RTypeMenuScenesButtonDisplay;
            }
            ~MenuState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void)
            {
                this->_ecs.registerComponent<RType::Components::Configuration>();
                this->_ecs.registerComponent<RType::Components::Action>();
                this->_ecs.registerComponent<RType::Components::Position>();
                this->_ecs.registerComponent<RType::Components::Sprite>();
                this->_ecs.registerComponent<Components::Buff>();
                this->_ecs.registerComponent<RType::Components::Hitbox>();
                this->_ecs.registerComponent<RType::Components::Relationship>();

                this->_ecs.setComponentFunction<RType::Components::Sprite>(RType::Components::Sprite::componentFunction);
                this->_ecs.setComponentFunction<RType::Components::Action>(RType::Components::Action::componentFunction);
                this->_ecs.setComponentFunction<RType::Components::Position>(RType::Components::Position::componentFunction);
                this->_ecs.setComponentFunction<RType::Components::Hitbox>(RType::Components::Hitbox::componentFunction);
            }
            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void)
            {
                // WIP
                if (this->_currentScene == RTypeMenuScenes::RTypeMenuScenesButtonDisplay) {
                    // Display button
                    this->_ecs.runComponentFunction<RType::Components::Position>();
                    this->_ecs.runComponentFunction<RType::Components::Sprite>();
                }
                return StateMenu;
            }
            const LobbyInfo& getLobbyInfo(void) const noexcept
            {
                return this->_lobbyInfo;
            }

            void makeButton(void){
                // WIP
                Rengine::Graphics::TextSpecs specs;

                specs.color = {0, 0, 255};
                specs.message = "";
                specs.fontPath = "assets/fonts/arial.ttf";

                this->_buttonTexts.push_back(Rengine::Graphics::GraphicManagerSingletone::get().createText(specs));

                //menu 1
                //champ de texte -> ip du serveur
                //champ de texte -> port du serveur

                //bouton -> connexion au serveur


                //menu 2 //lobby
                //liste (déroulante) -> liste des parties(nom de la game, nombre de joueurs, nombre de places,depuis quand la partie est ouverte)
                //bouton -> rejoindre la partie

                //bouton create game -> créer une partie (en bas de la page)
            }

        private:
            LobbyInfo _lobbyInfo;
            RType::RTypeMenuScenes _currentScene = RType::RTypeMenuScenes::RTypeMenuScenesNA;
            std::vector<std::shared_ptr<Rengine::Graphics::AText>> _buttonTexts;
    };

}  // namespace RType

#endif  // SRC_STATE_MENUSTATE_HPP_
