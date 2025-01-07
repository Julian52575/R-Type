#pragma once

#include <unordered_map>
#include <vector>
#include <functional>

#include <rengine/Rengine.hpp>

namespace RType {
    class SceneManagerExceptionSceneNotFound : public std::exception {
        public:
            const char *what() const noexcept { return "SceneManager: Scene not found."; };
    };

    class SceneManagerExceptionNoCurrentScene : public std::exception {
        public:
            const char *what() const noexcept { return "SceneManager: No current scene set."; };
    };

    template <typename Scene>
    class SceneManager {
        public:
            SceneManager(void) = default;
            ~SceneManager(void) = default;

            void addScene(Scene scene, std::function<void()> sceneFunction){
                this->_scenes[scene] = std::move(sceneFunction);
                this->_scenes_entities[scene] = std::vector<Rengine::Entity>();
                if (!this->_has_current_scene){
                    this->_current_scene = scene;
                    this->_has_current_scene = true;
                }
            }

            void addEntityToScene(Scene scene, Rengine::Entity &entity){
                if (this->_scenes_entities.find(scene) == this->_scenes_entities.end())
                    throw SceneManagerExceptionSceneNotFound();
                this->_scenes_entities[scene].push_back(entity);
            }

            void changerScene(Scene scene){
                if (this->_scenes.find(scene) == this->_scenes.end())
                    throw SceneManagerExceptionSceneNotFound();
                if (this->_current_scene == scene)
                    return;
                this->_current_scene = scene;
            }

            std::vector<Rengine::Entity>& getEntities(Scene scene){
                if (this->_scenes_entities.find(scene) == this->_scenes_entities.end())
                    throw SceneManagerExceptionSceneNotFound();
                return this->_scenes_entities[scene];
            }

            std::vector<Rengine::Entity>& getCurrentSceneEntities(void){
                if (!this->_has_current_scene)
                    throw SceneManagerExceptionNoCurrentScene();
                return this->_scenes_entities[this->_current_scene];
            }

            Scene getCurrentScene(void) const noexcept {
                if (!this->_has_current_scene)
                    throw SceneManagerExceptionNoCurrentScene();
                return this->_current_scene;
            }

            void callCurrentSceneFunction(void){
                if (!this->_has_current_scene)
                    throw SceneManagerExceptionNoCurrentScene();
                this->_scenes[this->_current_scene]();
            }

        private:
            std::unordered_map<Scene, std::vector<Rengine::Entity>> _scenes_entities;
            std::unordered_map<Scene, std::function<void()>> _scenes;
            Scene _current_scene;
            bool _has_current_scene = false;
    };

}