#include "src/State/LobbyState.hpp"

namespace RType {
    LobbyState::LobbyState(Rengine::ECS& ecs) : AState(ecs)
    {
        this->_client = nullptr;
        this->initScenes();
        this->_sceneManager.setScene(LobbyScenes::LobbySceneInitConnexion);
    }

    void LobbyState::registerComponents(void) {
    }

    State initLobbyConnexion(LobbyState &LobbyState) {
        try {
            LobbyState._client = std::make_unique<ClientTCP<Network::Communication::TypeDetail>>(LobbyState._lobbyInfo.serverIp, LobbyState._lobbyInfo.port);
        } catch (const std::exception& e) {
            std::cerr << "Error" << e.what() << std::endl;
            return State::StateMenu;
        }
        LobbyState._sceneManager.setScene(LobbyScenes::LobbySceneRun);
        return State::StateLobby;
    }

    State runLevel(LobbyState &LobbyState) {
        for (std::optional<Message<Network::Communication::TypeDetail>> msg = LobbyState._client->Receive(); msg; msg = LobbyState._client->Receive()) {
            std::cout << "Message received: " << msg->header.size << std::endl;
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
}
