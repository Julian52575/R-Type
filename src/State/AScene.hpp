//
#pragma once

#include "src/Game/SceneManager.hpp"
namespace RType {

    /**
    * @addtogroup RType
    * @namespace RType
    * @class AScene
    * @template SceneType The scene type used to differenciate between each scenes.
    * @template Parameters The Parameters to pass to the display function.
    * @brief An abstract class for the different scenes to follow.
    */
    template <typename SceneType, typename ... Parameters>
    class AScene {
        public:
            explicit AScene(SceneType sceneIndex)
                : _sceneIndex(sceneIndex)
            {
            }
            ~AScene(void) = default;
            /**
            * @fn unload
            * @brief Unload some of the scene's elements such as music...
            */
            virtual void unload(void) = 0;
            /**
            * @fn reload
            * @brief Reload some of the scene's elements such as music...
            */
            virtual void reload(void) = 0;
            /**
            * @fn run
            * @brief Run a frame of the scene.
            */
            virtual void display(Parameters ... params) = 0;
            /**
            * @fn handleInputs
            * @brief Handle inputs.
            */
            virtual SceneType handleInputs(void) = 0;

        protected:
            SceneType _sceneIndex;
    };

}  // namespace RType
