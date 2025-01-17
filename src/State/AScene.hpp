//
#pragma once

#include "src/Game/SceneManager.hpp"
namespace RType {

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
            virtual void unload(void);
            /**
            * @fn reload
            * @brief Reload some of the scene's elements such as music...
            */
            virtual void reload(void);
            /**
            * @fn run
            * @brief Run a frame of the scene.
            */
            virtual void display(Parameters ... params) = 0;
            /**
            * @fn handleInputs
            * @brief Handle inputs
            */
            virtual SceneType handleInputs(void) = 0;

        protected:
            SceneType _sceneIndex;
    };

}  // namespace RType
