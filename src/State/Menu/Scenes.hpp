//
#pragma once
namespace RType {

    enum MenuScenes {
        MenuScenesNA,
        MenuScenesAttractMode,
        MenuScenesTitleScreen,
        MenuScenesOptions,
        MenuScenesSelectCharacter,
        MenuScenesSelectLobby,
        MenuScenesButtonDisplay,
        MenuScenesExitToLobby
    };

#define MENUSCENES_MAX MenuScenes::MenuScenesExitToLobby

}  // namespace RType
