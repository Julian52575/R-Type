//
#ifndef SRC_STATE_LOBBYSTATE_HPP_
#define SRC_STATE_LOBBYSTATE_HPP_

#include <rengine/Rengine.hpp>
#include <rengine/RengineNetworkClient.hpp>

#include "AState.hpp"
#include "GameState.hpp"
#include "src/State/State.hpp"
#include "src/Network/EntityAction.hpp"
#include "GameState.hpp"

#include <ctime>

namespace RType {

    enum LobbyScenes {
        LobbySceneInitConnexion,
        LobbySceneRun
    };

    struct LobbyInfo {
        std::string serverIp = "127.0.0.1";
        uint16_t port = 4242;
        std::string playerJson = "assets/entities/playerDefault.json";
    };

    struct GameInfo {
        std::string name;
        Rengine::UUID::uuid_t id;
        uint16_t playerCount;
        time_t time;
    };

    struct DisplayGameInfo {
        std::shared_ptr<Rengine::Graphics::AText> name;
        std::shared_ptr<Rengine::Graphics::AText> playerCount;
        std::shared_ptr<Rengine::Graphics::AText> time;
        std::shared_ptr<Rengine::Graphics::ASprite> background;
        GameInfo Infos;
    };

    class LobbyState : public AState {
        public:
            LobbyState(Rengine::ECS& ecs);
            ~LobbyState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void);
            friend State initLobbyConnexion(LobbyState &LobbyState);

            friend State runLevel(LobbyState &LobbyState);

            void initScenes(void);

            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void);

            /*
            * @fn setLobbyInfo
            * @param lobbyInfo The data of the lobby.
            * @brief Set the lobbyInfo from the menu by the StateManager.
            */
            void setLobbyInfo(const LobbyInfo& lobbyInfo) noexcept;
            void handleInput(void);
            void makeGameInfos(std::string name, uint16_t playerCount, time_t time, uuid_t id);
            void setGameInfos(void);
            DisplayGameInfo& getGameInfoByUuid(Rengine::UUID::uuid_t &id);
            void updateGameInfos(void);
            NetworkInfo &getNetworkInfo(void) noexcept;

            float _time = 0;
            size_t _begin = 0;
            std::vector<DisplayGameInfo> _displayGameInfos;
            std::shared_ptr<Rengine::Graphics::ASprite> _cursor;

        private:
            LobbyInfo _lobbyInfo;
            NetworkInfo _networkInfo;
            std::unique_ptr<ClientTCP<Network::Communication::TypeDetail>> _client;
    };

}  // namespace RType

#endif  // SRC_STATE_LOBBYSTATE_HPP_
