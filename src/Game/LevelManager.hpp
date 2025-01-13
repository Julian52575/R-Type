#pragma once

#include "src/Config/LevelConfig.hpp"
#include "src/Config/LevelConfigResolver.hpp"

class LevelManager
{
    public:
        LevelManager(){};
        ~LevelManager(){};

        void loadLevel(const std::string& jsonPath){
            RType::Config::LevelConfigResolver& resolver = RType::Config::LevelConfigResolverSingletone::get();
            this->_config = resolver.get(jsonPath);
            this->_scenes = this->_config.getScenes();
            loadScene(0);
        };

        bool loadScene(int num){
            if (num >= _scenes.size() || num < 0){
                return false;
            }
            this->_current_scene_index = num;
            this->_current_scene = _scenes[num];
            this->_time = 0;
            return true;
        };

        void update(float deltaTime){
            this->_time += deltaTime;
        };

        bool SceneEndCondition(){
            if (this->_current_scene.endCondition == RType::Config::SceneEndCondition::SceneEndConditionTime){
                if (_time >= this->_current_scene.endConditionData.time){
                    return true;
                }
            }

            if (this->_current_scene.endCondition == RType::Config::SceneEndCondition::SceneEndConditionBossDefeat){
            }

            return false;
        };

        bool nextScene(){
            return loadScene(this->_current_scene_index + 1);
        };

        bool previousScene(){
            return loadScene(this->_current_scene_index - 1);
        };

        bool rebootScene(){
            return loadScene(this->_current_scene_index);
        };

        std::vector<RType::Config::SceneEntityConfig>& getCurrentSceneEnemies(){
            return this->_current_scene.enemies;
        };
        
        std::vector<RType::Config::ImageConfig>& getCurrentSceneBackgroundImages(){
            return this->_current_scene.backgroundImages;
        };


    private:
        RType::Config::LevelConfig _config;
        std::vector<RType::Config::SceneConfig> _scenes;
        int _current_scene_index = 0;
        RType::Config::SceneConfig _current_scene;
        float _time = 0;
};