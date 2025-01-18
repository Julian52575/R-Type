//
#pragma once
namespace RType {

    enum MenuScenes {
        MenuScenesNA,
        MenuScenesTitleScreen,
        MenuScenesOptions,
        MenuScenesSelectCharacter,
        MenuScenesSelectLobby,
        MenuScenesEnterLobbyInfo,
        MenuScenesCreateLobby,
        MenuScenesExitToGame,
        MenuScenesExitToLobby
    };

#define MENUSCENES_MAX MenuScenes::MenuScenesExitToLobby

}  // namespace RType
