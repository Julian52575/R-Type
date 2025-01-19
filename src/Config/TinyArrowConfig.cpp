#include "TinyArrowConfig.hpp"
#include <nlohmann/json.hpp>

#include "ImageConfig.hpp"

namespace Undertale {

    namespace Config {

        TinyArrowSpec::TinyArrowSpec(Direction direction, float velocity, float startTime, const Rengine::Graphics::vector2D<float>& vec)
            : _direction(direction), _velocity(velocity), _startTime(startTime)
        {
            Rengine::Graphics::SpriteSpecs specs;
            auto manager = Rengine::Graphics::GraphicManagerSingletone::get();
            _rect = FloatRect(vec.y, vec.x, 0, 0);

            if (direction == UP) {
                specs.texturePath = "assets/images/arrows_up.png";
                _rect._width = 13;
                _rect._height = 25;
            }
            if (direction == DOWN) {
                specs.texturePath = "assets/images/arrows_down.png";
                _rect._width = 13;
                _rect._height = 25;
            }
            if (direction == LEFT) {
                specs.texturePath = "assets/images/arrows_left.png";
                _rect._width = 25;
                _rect._height = 13;
            }
            if (direction == RIGHT) {
                specs.texturePath = "assets/images/arrows_right.png";
                _rect._width = 25;
                _rect._height = 13;
            }
            this->_sprite = manager.createSprite(specs);
        }

        bool TinyArrowSpec::collide(const FloatRect& rect) const
        {
            return _rect.collidesWith(rect);
        }

        void TinyArrowSpec::draw(void) const noexcept
        {
            auto manager = Rengine::Graphics::GraphicManagerSingletone::get();
            manager.addToRender(_sprite, {_rect._left, _rect._top});
        }

        void TinyArrowSpec::update(float delta)
        {
            if (_direction == UP) {
                _rect._top -= delta * _velocity;
            }
            if (_direction == DOWN) {
                _rect._top += delta * _velocity;
            }
            if (_direction == LEFT) {
                _rect._left += delta * _velocity;
            }
            if (_direction == RIGHT) {
                _rect._left -= delta * _velocity;
            }
        }

        TinyArrowSpec::Direction TinyArrowSpec::getDir(void) const
        {
            return this->_direction;
        }

        float TinyArrowSpec::getTime(void) const {
            return _startTime;
        }

        TinyArrowConfig::TinyArrowConfig(const std::string& jsonPath)
        {
            std::ifstream f;

            try {
                f.open(jsonPath);
            } catch (std::exception &e) {
                throw TinyArrowConfigExceptionFileError(jsonPath, e.what());
            }
            nlohmann::json j;

            try {
                j = nlohmann::json::parse(f);
            } catch (std::exception &e) {
                throw TinyArrowConfigExceptionInvalidJsonFile(jsonPath, e.what());
            }
            if (j.contains("tinyArrow") == false) {
                throw TinyArrowConfigExceptionInvalidJsonFile(jsonPath, "No 'tinyArrow' field.");
            }
            j = j["tinyArrow"];
            this->parseTinyArrow(j);
            this->createPlayer();
        }

        void TinyArrowConfig::parseTinyArrow(const nlohmann::json& j)
        {
            for (const auto& item : j) {
                if (!item.contains("direction") || !item.contains("velocity")
                    || !item.contains("startTime") || !item.contains("pos_x") || !item.contains("pos_y")) {
                    throw TinyArrowConfigExceptionInvalidJsonFile("assets/attacks/", "Invalid tinyArrow entry.");
                }

                TinyArrowSpec::Direction direction = item["direction"];
                float velocity = item["velocity"];
                float startTime = item["startTime"];
                Rengine::Graphics::vector2D<float> pos = {item["pos_x"], item["pos_y"]};

                this->_availableArrows.push_back(std::make_unique<TinyArrowSpec>(direction, velocity, startTime, pos));
            }
        }

        bool TinyArrowConfig::checkCollision(const FloatRect& bounds)
        {
            int index = 0;
            int toDelete = -1;
            for (const auto& it : _tinyArrows) {
                if (it->collide(bounds)) {
                    if (_dir == TinyArrowSpec::Direction::UP && it->getDir() == TinyArrowSpec::Direction::DOWN)
                        toDelete = index;
                    if (_dir == TinyArrowSpec::Direction::DOWN && it->getDir() == TinyArrowSpec::Direction::UP)
                        toDelete = index;
                    if (_dir == TinyArrowSpec::Direction::LEFT && it->getDir() == TinyArrowSpec::Direction::RIGHT)
                        toDelete = index;
                    if (_dir == TinyArrowSpec::Direction::RIGHT && it->getDir() == TinyArrowSpec::Direction::LEFT)
                        toDelete = index;
                }
                if (it->collide(_playerRect)) {
                    _tinyArrows.erase(_tinyArrows.begin() + index);
                    return true;
                }
                index++;
            }

            if (toDelete != -1) {
                _tinyArrows.erase(_tinyArrows.begin() + toDelete);
            }
            return false;
        }

        void TinyArrowConfig::createPlayer(void)
        {
            auto manager = Rengine::Graphics::GraphicManagerSingletone::get();

            _playerRect = FloatRect(225, 305 , 20, 20);
            _player = manager.createSprite(ImageConfig("assets/images/heart.json").getSpecs());

            _defRect = FloatRect(203, 283, 66, 3); // 200 280 70 70
            Rengine::Graphics::SpriteSpecs specs;
            specs.type = Rengine::Graphics::SpriteTypeRectangle;
            specs.color = {255, 0, 0};
            specs.shapeData.outlineThickness = 0;
            specs.shapeData.specifics.rectangleSize = {_defRect._width, _defRect._height};
            _playerDef = manager.createSprite(specs);
        }

        void TinyArrowConfig::draw(void)
        {
            auto manager = Rengine::Graphics::GraphicManagerSingletone::get();
            manager.addToRender(_player, {_playerRect._left, _playerRect._top});
            manager.addToRender(_playerDef, {_defRect._left, _defRect._top});
            for (const auto& it : _tinyArrows) {
                it->draw();
            }
        }

        void TinyArrowConfig::update(void)
        {
            Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
            inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardChar);
            inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecial);

            sf::Time elaspedTime = _gameClock.getElapsedTime();
            _arrowTime += elaspedTime;
            _inputTime += elaspedTime;
            _overallTime += elaspedTime;

            if (_inputTime.asSeconds() > 0.02) {
                for (auto it : inputManager) {
                    if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowUP) {
                        _dir = TinyArrowSpec::UP;
                        _defRect = FloatRect(203, 283, 66, 3); // 200 280 70 70
                    }
                    if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowDOWN) {
                        _dir = TinyArrowSpec::DOWN;
                        _defRect = FloatRect(267, 283, 66, 3); // 200 280 70 70
                    }
                    if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowLEFT) {
                        _dir = TinyArrowSpec::LEFT;
                        _defRect = FloatRect(203, 283, 3, 66); // 200 280 70 70
                    }
                    if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowRIGHT) {
                        _dir = TinyArrowSpec::RIGHT;
                        _defRect = FloatRect(203, 347, 3, 66); // 200 280 70 70
                    }
                    Rengine::Graphics::SpriteSpecs specs = _playerDef->getSpriteSpecs();
                    specs.shapeData.specifics.rectangleSize = {_defRect._width, _defRect._height};
                    _playerDef->updateSpriteSpecs(specs);
                    _inputTime = sf::Time::Zero;
                }
            }
            if (_arrowTime.asMilliseconds() > 10) {
                for (const auto& arrow : _tinyArrows) {
                    arrow->update(_arrowTime.asMilliseconds());
                }
                _arrowTime = sf::Time::Zero;
            }
            for (auto it = _availableArrows.begin(); it != _availableArrows.end(); ) {
                if (_overallTime.asSeconds() > (*it)->getTime() + 1.5) {
                    _tinyArrows.push_back(std::move(*it));
                    it = _availableArrows.erase(it);
                } else {
                    ++it;
                }
            }
            _gameClock.restart();
        }

        bool TinyArrowConfig::attackOver(void)
        {
            if (_tinyArrows.empty() && _availableArrows.empty()) {
                return true;
            }
            return false;
        }
    }
}
