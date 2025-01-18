//
#include <climits>
#include <exception>
#include <filesystem>
#include <functional>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/AWindow.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <rengine/src/Rng.hpp>

#include "State.hpp"
#include "GameState.hpp"
#include "AState.hpp"

namespace Undertale {

    GameState::GameState(Rengine::ECS& ecs)
        : AState(ecs)
    {
        this->initScenes();
        this->_sceneManager.setScene(GameScenes::GameScenesPlay);
    }

    void GameState::registerComponents(void)
    {
    }

    void GameState::loadCurrentScene()
    {
    }

    State GameState::run(void)
    {
        State s = this->_sceneManager.callCurrentSceneFunction<State, GameState&>(*this);

        return s;
    }

    void GameState::deletePlayer(void)
    {
    }

    void destroyEntity(Rengine::ECS& ecs, Rengine::Entity& entity)
    {
    }

    State playFunction(GameState& gameState)
    {
        auto manager = Rengine::Graphics::GraphicManagerSingletone::get();
        manager.addToRender(gameState._undyne, gameState.getSpriteVecFloat(gameState._undyne));

        manager.addToRender(gameState._act_unselected, gameState.getSpriteVecFloat(gameState._act_unselected));
        manager.addToRender(gameState._fight_unselected, gameState.getSpriteVecFloat(gameState._fight_unselected));
        manager.addToRender(gameState._item_unselected, gameState.getSpriteVecFloat(gameState._item_unselected));
        manager.addToRender(gameState._mercy_unselected, gameState.getSpriteVecFloat(gameState._mercy_unselected));

        return StateGame;
    }

    void GameState::initScenes(void)
    {
        std::function<State(GameState&)> funScene(playFunction);

        this->_sceneManager.setSceneFunction<State, GameState&>(GameScenes::GameScenesPlay, funScene);
        initScenePlay();
    }

    void GameState::initScenePlay(void)
    {
        Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();

        auto _undyneConfig = Config::ImageConfig("assets/images/undyne.json");
        _undyne = manager.createSprite(_undyneConfig.getSpecs());

        _act_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_act.json", "assets/images/button_act_unused.png").getSpecs());
        _act_selected = manager.createSprite(Config::ImageConfig("assets/images/button_act.json", "assets/images/button_act_used.png").getSpecs());

        _fight_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_fight.json", "assets/images/button_fight_unused.png").getSpecs());
        _fight_selected = manager.createSprite(Config::ImageConfig("assets/images/button_fight.json", "assets/images/button_fight_used.png").getSpecs());

        _item_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_item.json", "assets/images/button_item_unused.png").getSpecs());
        _item_selected = manager.createSprite(Config::ImageConfig("assets/images/button_item.json", "assets/images/button_item_used.png").getSpecs());

        _mercy_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_mercy.json", "assets/images/button_mercy_unused.png").getSpecs());
        _mercy_selected = manager.createSprite(Config::ImageConfig("assets/images/button_mercy.json", "assets/images/button_mercy_used.png").getSpecs());
    }

    void GameState::createPlayer(const std::string& jsonPath)
    {
    }

    void GameState::alertPlayer(void)
    {
    }

    Rengine::Graphics::vector2D<float> GameState::getSpriteVecFloat(std::shared_ptr<Rengine::Graphics::ASprite> sprite)
    {
        return {static_cast<float>(sprite->getSpriteSpecs().originOffset.x), static_cast<float>(sprite->getSpriteSpecs().originOffset.y)};
    }

}  // namespace Undertale
