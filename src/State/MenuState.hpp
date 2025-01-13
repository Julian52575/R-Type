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

                    this->_time += Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getDeltaTimeSeconds();
                    this->inputTextbox(this->_buttonTexts[this->idx_selected]);

                    Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_backgroundSprites[0], {0, static_cast<float>(this->idx_selected * 50 + 25)});
                    Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_buttonTexts[0], {0, 0});
                    Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_buttonTexts[1], {0, 50});


                }
                return StateMenu;
            }
            const LobbyInfo& getLobbyInfo(void) const noexcept
            {
                return this->_lobbyInfo;
            }

            void inputTextbox(std::shared_ptr<Rengine::Graphics::AText> &textBox)
            {
                if (this->_time < 0.08)
                    return;
                this->_time = 0;

                Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
                std::string currentText = textBox->getText();

                for (auto it : inputManager) {
                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardSpecial && it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialTAB) {
                        this->idx_selected = (this->idx_selected + 1) % this->_buttonTexts.size();
                        continue;
                    }

                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardSpecial && it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialBACKSPACE) {
                        if (currentText.size() > 0) {
                            currentText.pop_back();
                        }
                        continue;
                    }
                    if (it.type != Rengine::Graphics::UserInputTypeKeyboardChar)
                        continue;
                    currentText += it.data.keyboardChar;
                }
                textBox->setText(currentText);
            }

            void makeButton(void){
                // WIP
                Rengine::Graphics::TextSpecs specs;

                specs.color = {0, 0, 255};
                specs.message = "addresse ip : ";
                specs.fontPath = "assets/fonts/arial.ttf";
                this->_buttonTexts.push_back(Rengine::Graphics::GraphicManagerSingletone::get().createText(specs));

                specs.color = {255, 0, 0};
                specs.message = "port :";
                this->_buttonTexts.push_back(Rengine::Graphics::GraphicManagerSingletone::get().createText(specs));


                Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
                Rengine::Graphics::SpriteSpecs spriteSpecs;

                spriteSpecs.type = Rengine::Graphics::SpriteTypeRectangle;

                spriteSpecs.color = {0, 0, 0};
                spriteSpecs.shapeData.outlineColor = {255, 0, 255};
                spriteSpecs.shapeData.outlineThickness = 5;
                spriteSpecs.shapeData.specifics.rectangleSize = {200, 50};
                this->_backgroundSprites.push_back(manager.createSprite(spriteSpecs));

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
            std::vector<std::shared_ptr<Rengine::Graphics::ASprite>> _backgroundSprites;
            float _time = 0;
            int idx_selected = 0;
    };

}  // namespace RType

#endif  // SRC_STATE_MENUSTATE_HPP_
