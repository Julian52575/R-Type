#ifndef SRC_GAME_SCENEMANAGER_HPP_
#define SRC_GAME_SCENEMANAGER_HPP_

#include <any>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <unordered_map>
#include <vector>
#include <functional>
#include <utility>
#include <rengine/Rengine.hpp>

namespace RType {
    class SceneManagerExceptionSceneNotFound : public std::exception {
        public:
            const char *what() const noexcept { return "RType::SceneManager: Scene not found."; };
    };
    class SceneManagerExceptionNoCurrentScene : public std::exception {
        public:
            const char *what() const noexcept { return "RType::SceneManager: No current scene set."; };
    };
    class SceneManagerExceptionBadParametersTemplate : public std::exception {
        public:
            const char *what() const noexcept { return "RType::SceneManager: Function prototype is different from provided template."; };
    };
    /**
    * @addtogroup RType::Config
    * @namespace RType
    * @class SceneManager
    * @brief Antoine.
    */
    class SceneManager {
        public:
            using scene_type = uint64_t;
            SceneManager(void) = default;
            ~SceneManager(void) = default;
            /*
            * @fn setSceneFunction
            * @template ReturnType The return type of the scene's function.
            * @template Parameters The parameters of the function
            * @param scene The scene identifier
            * @param sceneFunction The scene's function.
            * @brief Bind a function to a scene.
            * Overwrite the function if it was previously set.
            * Does not remove the entities present in the scene.
            */
            template <class ReturnType, class ... Parameters>
            void setSceneFunction(scene_type scene, const std::function<ReturnType(Parameters&&...)>& sceneFunction)
            {
                // Create the sceneData if not created previously
                if (this->_scenesData .contains(scene) == false) {
                    this->_scenesData[scene] = {{0}, {0}};
                }
                this->_scenesData[scene]._sceneFunction = sceneFunction;
            }
            /*
            * @fn addEntityToCurrentScene
            * @param entityId The id of the entity to add.
            * @brief Adds an entity to the current scene.
            */
            void addEntityToCurrentScene(Rengine::Entity::size_type entityId)
            {
                return this->addEntityToScene(this->_currentScene, entityId);
            }
            /*
            * @fn addEntityToCurrentScene
            * @param entityId The id of the entity to add.
            * @brief Adds an entity to the requested scene.
            * Data for the scene is created if it wasn't created previously
            */
            void addEntityToScene(scene_type scene, Rengine::Entity::size_type entityId)
            {
                // Create the sceneData if not created previously
                if (this->_scenesData.find(scene) == this->_scenesData.end()) {
                    this->_scenesData[scene] = {{0}, {0}};
                }
                // Check if entity is already listed
                for (auto& id : this->_scenesData[scene]._sceneEntities) {
                    if (id == entityId) {
                        return;
                    }
                }
                // Add entity
                this->_scenesData[scene]._sceneEntities.push_back(entityId);
            }
            /*
            * @fn setScene
            * @param scene The new scene.
            * @exception SceneManagerExceptionSceneNotFound The asked scene was not added to the manager previously.
            * @brief Change the current scene.
            */
            void setScene(scene_type scene)
            {
                // No scene
                if (this->_scenesData.find(scene) == this->_scenesData.end()) {
                    throw SceneManagerExceptionSceneNotFound();
                }
                this->_currentScene = scene;
            }
            /*
            * @fn getCurrentScene
            * @return scene The current scene.
            * @brief Get the currently set scene.
            */
            scene_type getCurrentScene(void) const noexcept
            {
                return this->_currentScene;
            }
            /*
            * @fn getEntity
            * @param scene The scene to query.
            * @exception SceneManagerExceptionSceneNotFound The asked scene was not added to the manager previously.
            * @return A vector of the entities id.
            * @brief Retrive the scene's entities ids.
            */
            const std::vector<Rengine::ECS::size_type>& getEntities(scene_type scene)
            {
                // No scene
                if (this->_scenesData.find(scene) == this->_scenesData.end()) {
                    throw SceneManagerExceptionSceneNotFound();
                }
                return this->_scenesData[scene]._sceneEntities;
            }
            /*
            * @fn getEntity
            * @param scene The scene to query.
            * @exception SceneManagerExceptionSceneNotFound The current scene cannot be accessed.
            * @return A vector of the entities id.
            * @brief Retrive the current scene's entities ids.
            */
            const std::vector<Rengine::ECS::size_type>& getCurrentSceneEntities(void)
            {
                return this->getEntities(this->_currentScene);
            }
            /*
            * @fn callCurrentSceneFunction
            * @template ReturnType The return type of the scene's function.
            * @template Parameters The parameters of the scene's function.
            * @exception SceneManagerExceptionSceneNotFound The current scene cannot be accessed.
            * @exception SceneManagerExceptionBadParametersTemplate The template parameters are different from the function's prototype.
            * @exception std::bad_function_call The current scene has no associated function.
            * @brief Execute the current scene's function.
            */
            template <class ReturnType, class ... Parameters>
            ReturnType callCurrentSceneFunction(Parameters&& ... params)
            {
                // No scene
                if (this->_scenesData.contains(this->_currentScene) == false) {
                    throw SceneManagerExceptionSceneNotFound();
                }
                try {
                    std::function<ReturnType(Parameters&&...)> fun
                        = std::any_cast<std::function<ReturnType(Parameters&& ...)>>(this->_scenesData[this->_currentScene]._sceneFunction);

                    return fun(std::forward<Parameters>(params)...);
                } catch (std::bad_any_cast& e) {
                    throw SceneManagerExceptionBadParametersTemplate();
                }
            }

        private:
            struct SceneData {
                std::vector<Rengine::ECS::size_type> _sceneEntities;
                std::any _sceneFunction;  // std::function<void(???)>
            };
            std::unordered_map<scene_type, SceneData> _scenesData;
            scene_type _currentScene;
            bool _hasCurrentScene = false; // wtf
    };

}  // namespace RType
#endif  // SRC_GAME_SCENEMANAGER_HPP_

