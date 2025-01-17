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
        MenuScenesExitToGame,
        MenuScenesExitToLobby
    };

#define MENUSCENES_MAX MenuScenes::MenuScenesExitToLobby

}  // namespace RType
