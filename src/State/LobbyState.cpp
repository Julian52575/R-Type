#include "src/State/LobbyState.hpp"

namespace RType {
    LobbyState::LobbyState(Rengine::ECS& ecs) : AState(ecs)
    {
        this->_client = nullptr;
        this->initScenes();
        this->_sceneManager.setScene(LobbyScenes::LobbySceneInitConnexion);
        this->setGameInfos();
    }

    void LobbyState::registerComponents(void) {
    }

    State initLobbyConnexion(LobbyState &LobbyState) {
        //mettre le try catch en com pour disable le server
        // try {
        //     LobbyState._client = std::make_unique<ClientTCP<Network::Communication::TypeDetail>>(LobbyState._lobbyInfo.serverIp, LobbyState._lobbyInfo.port);
        // } catch (const std::exception& e) {
        //     std::cerr << "Error" << e.what() << std::endl;
        //     return State::StateMenu;
        // }
        LobbyState._sceneManager.setScene(LobbyScenes::LobbySceneRun);
        return State::StateLobby;
    }

    State runLevel(LobbyState &LobbyState) {
        //mettre le for en com pour disable le server
        // for (std::optional<Message<Network::Communication::TypeDetail>> msg = LobbyState._client->Receive(); msg; msg = LobbyState._client->Receive()) {
        //     std::cout << "Message received: " << msg->header.size << std::endl;
        // }
        LobbyState._time += Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getDeltaTimeSeconds();
        LobbyState.handleInput();
        for (size_t i = LobbyState._begin; i < LobbyState._displayGameInfos.size() && i < LobbyState._begin + 5; i++) {
            float pos_y = 100.0f + 70.0f * static_cast<float>(i - LobbyState._begin);
            float pos_x = 1920/2 - 325.0f;
            if (i == LobbyState._begin)
                Rengine::Graphics::GraphicManagerSingletone::get().addToRender(LobbyState._cursor, {pos_x - 20.0f, pos_y});
            else
                Rengine::Graphics::GraphicManagerSingletone::get().addToRender(LobbyState._displayGameInfos[i].background, {pos_x - 20.f, pos_y});
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(LobbyState._displayGameInfos[i].name, {pos_x, pos_y});
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(LobbyState._displayGameInfos[i].playerCount, {pos_x + 250.0f, pos_y});
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(LobbyState._displayGameInfos[i].time, {pos_x + 450.0f, pos_y});
        }

        return State::StateLobby;
    }

    void LobbyState::initScenes(void) {
        std::function<State(LobbyState&)> funScene1(initLobbyConnexion);
        this->_sceneManager.setSceneFunction<State, LobbyState&>(LobbyScenes::LobbySceneInitConnexion, funScene1);

        std::function<State(LobbyState&)> funScene2(runLevel);
        this->_sceneManager.setSceneFunction<State, LobbyState&>(LobbyScenes::LobbySceneRun, funScene2);
    }


    State LobbyState::run(void)
    {
        State s = this->_sceneManager.callCurrentSceneFunction<State, LobbyState&>(*this);
        return s;
    }

    void LobbyState::setLobbyInfo(const LobbyInfo& lobbyInfo) noexcept
    {
        this->_lobbyInfo = lobbyInfo;
    }

    void LobbyState::handleInput(void)
    {
        if (this->_time < 0.10f) {
            return;
        }
        this->_time = 0;
        Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
        for (auto it : inputManager) {
            if (it.type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
                switch (it.data.keyboardSpecial) {
                    case Rengine::Graphics::UserInputKeyboardSpecialArrowUP:
                        if (this->_begin > 0)
                            this->_begin--;
                        break;
                    case Rengine::Graphics::UserInputKeyboardSpecialArrowDOWN:
                        this->_begin++;
                        if (this->_begin >= this->_displayGameInfos.size())
                            this->_begin = this->_displayGameInfos.size() - 1;
                        break;
                    case Rengine::Graphics::UserInputKeyboardSpecialENTER:
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void LobbyState::makeGameInfos(std::string name, uint16_t playerCount, time_t time, uuid_t id){
        GameInfo gameInfo;
        gameInfo.name = name;
        gameInfo.playerCount = playerCount;
        gameInfo.time = time;
        uuid_copy(gameInfo.id, id);

        DisplayGameInfo displayGameInfo;
        displayGameInfo.Infos = gameInfo;

        std::tuple<std::shared_ptr<Rengine::Graphics::AText>, std::shared_ptr<Rengine::Graphics::AText>, std::shared_ptr<Rengine::Graphics::AText>> gameTexts;
        Rengine::Graphics::TextSpecs specs;
        specs.fontPath = "assets/fonts/arial.ttf";
        specs.color = {255, 0, 0};
        specs.characterSize /= 2;
        specs.style = Rengine::Graphics::TextStyle::TextStyleRegular;

        specs.message = gameInfo.name;
        displayGameInfo.name = Rengine::Graphics::GraphicManagerSingletone::get().createText(specs);
        specs.message = "Joueurs : " +  std::to_string(gameInfo.playerCount);
        displayGameInfo.playerCount = Rengine::Graphics::GraphicManagerSingletone::get().createText(specs);
        specs.message = std::to_string(gameInfo.time);
        displayGameInfo.time = Rengine::Graphics::GraphicManagerSingletone::get().createText(specs);

        Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
        Rengine::Graphics::SpriteSpecs spriteSpecs;

        spriteSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
        spriteSpecs.color = {0, 0, 0};
        spriteSpecs.shapeData.outlineColor = {255, 255, 255};
        spriteSpecs.shapeData.outlineThickness = 5;
        spriteSpecs.shapeData.specifics.rectangleSize = {650, 50};
        displayGameInfo.background = manager.createSprite(spriteSpecs);

        this->_displayGameInfos.push_back(displayGameInfo);
    };

    void LobbyState::setGameInfos(void) {
        uuid_t id;
        uuid_generate(id);
        this->makeGameInfos("Eliott", 1, time(nullptr), id);
        this->makeGameInfos("partie 2 ", 10, time(nullptr), id);
        this->makeGameInfos("partie 3 ", 10, time(nullptr), id);
        this->makeGameInfos("partie 4 ", 10, time(nullptr), id);
        this->makeGameInfos("partie 5 ", 10, time(nullptr), id);
        this->makeGameInfos("partie 6 ", 10, time(nullptr), id);

        Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
        Rengine::Graphics::SpriteSpecs spriteSpecs;

        spriteSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
        spriteSpecs.color = {0, 0, 0};
        spriteSpecs.shapeData.outlineColor = {255, 0, 255};
        spriteSpecs.shapeData.outlineThickness = 5;
        spriteSpecs.shapeData.specifics.rectangleSize = {650, 50};

        this->_cursor = manager.createSprite(spriteSpecs);
        this->_begin = 0;
    }
}
