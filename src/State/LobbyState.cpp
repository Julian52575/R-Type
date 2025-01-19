#include "src/State/LobbyState.hpp"
#include "src/State/State.hpp"
#include <climits>
#include <rengine/src/CustomUUID.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/Rengine.hpp>

namespace RType {
    LobbyState::LobbyState(Rengine::ECS& ecs, LobbyInfo& lobbyInfo, NetworkInfo& networkInfo)
        : AState(ecs),
        _lobbyInfo(lobbyInfo),
        _networkInfo(networkInfo)
    {
        this->_client = nullptr;
        this->initScenes();
        this->_sceneManager.setScene(LobbyScenes::LobbySceneInitConnexion);
        this->setGameInfos();
    }

    void LobbyState::registerComponents(void) {
    }

    void LobbyState::updateGameInfos(void) {
        Message<Network::Communication::TypeDetail> msg;
        msg.header.size = 0;
        msg.header.type = {Network::Communication::Type::LobbyInfo, Network::Communication::main::LobbyInfoPrecision::RequestGamesInfo};
        this->_client->Send(msg);
    }

    State initLobbyConnexion(LobbyState &LobbyState) {
        //mettre le try catch en com pour disable le server
        try {
            LobbyState._client = std::make_unique<ClientTCP<Network::Communication::TypeDetail>>(LobbyState._lobbyInfo.serverIp, LobbyState._lobbyInfo.port);
        } catch (const std::exception& e) {
            std::cerr << "Error " << e.what() << " on " << LobbyState._lobbyInfo.serverIp << ":" << LobbyState._lobbyInfo.port << std::endl;
            return State::StateMenu;
        }
        Message<Network::Communication::TypeDetail> msg;
        msg.header.size = 0;
        msg.header.type = {Network::Communication::Type::LobbyInfo, Network::Communication::main::LobbyInfoPrecision::RequestGamesInfo};
        LobbyState._client->Send(msg);
        if (LobbyState._networkInfo.lobbyName == "") {
            LobbyState._sceneManager.setScene(LobbyScenes::LobbyScenesJoinLobby);
        } else {
            LobbyState._sceneManager.setScene(LobbyScenes::LobbyScenesCreateLobby);
        }
        return State::StateLobby;
    }

    State joinLobbyScene(LobbyState& lobbyState)
    {
        std::cout << "[connectToLobby] NetworkInfo: " << std::endl
            << "Name: " << lobbyState._networkInfo.lobbyName << std::endl
            << "IP:UDP:TCP :" << lobbyState._networkInfo.ip << ":" << lobbyState._networkInfo.UDPPort << ":" << lobbyState._networkInfo.TCPPort << std::endl;
        lobbyState._sceneManager.setScene(LobbyScenes::LobbySceneRun);
        return State::StateLobby;
    }
    State createLobbyScene(LobbyState& lobbyState)
    {
        std::cout << "[connectToLobby] NetworkInfo: " << std::endl
            << "Name: " << lobbyState._networkInfo.lobbyName << std::endl
            << "IP:UDP:TCP :" << lobbyState._networkInfo.ip << ":" << lobbyState._networkInfo.UDPPort << ":" << lobbyState._networkInfo.TCPPort << std::endl;
        lobbyState._sceneManager.setScene(LobbyScenes::LobbySceneRun);
        return State::StateLobby;
    }

    DisplayGameInfo &LobbyState::getGameInfoByUuid(Rengine::UUID::uuid_t &id) {
        for (auto &game : this->_displayGameInfos) {
            if (Rengine::UUID::compareUUID(game.Infos.id, id) == 0) {
                return game;
            }
        }
        throw std::runtime_error("Game not found");
    }


    State runLevel(LobbyState &LobbyState) {
        // mettre le for en com pour disable le server
        for (std::optional<Message<Network::Communication::TypeDetail>> msg = LobbyState._client->Receive(); msg; msg = LobbyState._client->Receive()) {
            std::cout << "Received message of type " << msg->header.type.type << " and precision " << msg->header.type.precision << std::endl;
            if (msg->header.type.type == Network::Communication::LobbyInfo && msg->header.type.precision == Network::Communication::main::LobbyInfoPrecision::GameInfo) {
                LobbyState._displayGameInfos.clear();
                uint16_t nbGames;
                *msg >> nbGames;
                for (uint16_t i = 0; i < nbGames; i++) {
                    Rengine::UUID::uuid_t gameID;
                    uint16_t nbUsers;
                    char name[15];
                    time_t timeStarted;
                    *msg >> timeStarted >> name >> nbUsers >> gameID;
                    std::string nameStr(name);
                    LobbyState.makeGameInfos(nameStr, nbUsers, timeStarted, gameID);
                }
            }
            if (msg->header.type.type == Network::Communication::LobbyInfo && msg->header.type.precision == Network::Communication::main::LobbyInfoPrecision::GameCreated) {
                Rengine::UUID::uuid_t gameID;
                uint16_t nbUsers;
                char name[15];
                time_t timeStarted;
                *msg >> timeStarted >> name >> nbUsers >> gameID;
                std::string nameStr(name);
                LobbyState.makeGameInfos(nameStr, nbUsers, timeStarted, gameID);
            }

            if (msg->header.type.type == Network::Communication::LobbyInfo && msg->header.type.precision == Network::Communication::main::LobbyInfoPrecision::GameUpdated) {
                Rengine::UUID::uuid_t gameID;
                uint16_t nbUsers;
                char name[15];
                time_t timeStarted;
                *msg >> timeStarted >> name >> nbUsers >> gameID;
                std::string nameStr(name);
                try {
                    DisplayGameInfo& game = LobbyState.getGameInfoByUuid(gameID);
                    game.Infos.name = nameStr;
                    game.Infos.playerCount = nbUsers;
                    game.Infos.time = timeStarted;
                } catch (const std::runtime_error& e) {
                    LobbyState.makeGameInfos(nameStr, nbUsers, timeStarted, gameID);
                }
            }

            if (msg->header.type.type == Network::Communication::LobbyInfo && msg->header.type.precision == Network::Communication::main::LobbyInfoPrecision::RequestedGame) {
                uint16_t gameTCPport;
                uint16_t gameUDPport;
                *msg >> gameUDPport >> gameTCPport;
                LobbyState._networkInfo.ip = LobbyState._lobbyInfo.serverIp;
                LobbyState._networkInfo.TCPPort = gameTCPport;
                LobbyState._networkInfo.UDPPort = gameUDPport;
                return StateGame;
            }
        }
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
            if (Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager()
            .receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecialPressed, {Rengine::Graphics::UserInputKeyboardSpecialENTER})) {
                Message<Network::Communication::TypeDetail> msg;
                msg.header.type = {Network::Communication::Type::LobbyInfo, Network::Communication::main::LobbyInfoPrecision::JoinGame};
                msg.header.size = 0;
                msg << LobbyState._displayGameInfos[i].Infos.id;
                LobbyState._client->Send(msg);
            }
        }
        return State::StateLobby;
    }

    void LobbyState::initScenes(void) {
        std::function<State(LobbyState&)> funScene1(initLobbyConnexion);
        this->_sceneManager.setSceneFunction<State, LobbyState&>(LobbyScenes::LobbySceneInitConnexion, funScene1);

        std::function<State(LobbyState&)> funScene2(runLevel);
        this->_sceneManager.setSceneFunction<State, LobbyState&>(LobbyScenes::LobbySceneRun, funScene2);

        std::function<State(LobbyState&)> create(createLobbyScene);
        this->_sceneManager.setSceneFunction<State, LobbyState&>(LobbyScenes::LobbyScenesCreateLobby, create);

        std::function<State(LobbyState&)> join(joinLobbyScene);
        this->_sceneManager.setSceneFunction<State, LobbyState&>(LobbyScenes::LobbyScenesJoinLobby, join);
    }


    State LobbyState::run(void)
    {
        State s = this->_sceneManager.callCurrentSceneFunction<State, LobbyState&>(*this);
        return s;
    }

    void LobbyState::handleInput(void)
    {
        if (this->_time < 0.05f) {
            return;
        }
        this->_time = 0;
        Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
        for (auto it : inputManager) {
            if (it.type == Rengine::Graphics::UserInputTypeKeyboardCharPressed && it.data.keyboardChar == 'r') {
                this->updateGameInfos();
                continue;
            }
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

    void LobbyState::makeGameInfos(std::string name, uint16_t playerCount, time_t time, Rengine::UUID::uuid_t &id){
        GameInfo gameInfo;
        gameInfo.name = name;
        gameInfo.playerCount = playerCount;
        gameInfo.time = time;
        Rengine::UUID::copyUUID(gameInfo.id, id);

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
        Rengine::UUID::uuid_t id;
        Rengine::UUID::generateUUID(id);

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
