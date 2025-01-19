#ifndef SRC_CONFIG_TINYARROWCONFIG_HPP_
#define SRC_CONFIG_TINYARROWCONFIG_HPP_

#include <rengine/Rengine.hpp>
#include <string>
#include <nlohmann/json.hpp>
#include <rengine/src/Graphics/SpriteSpecs.hpp>
#include <rengine/src/Graphics/ASprite.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/Vector.hpp>
#include <vector>
#include <fstream>
#include <memory>
#include <SFML/System/Clock.hpp>

namespace Undertale {

    namespace Config {

        class TinyArrowConfigExceptionFileError : public std::exception {
            public:
                TinyArrowConfigExceptionFileError(const std::string& fileName, const char *jsonMsg)
                {
                    std::string msg = "Undertale::Config::TinyArrowConfig An error happened when trying to read from '" + fileName + "'.";

                    this->_concat = msg + jsonMsg;
                };
                const char *what() const noexcept
                {
                    return this->_concat.c_str();
                };

            private:
                std::string _concat;
        };
        
        class TinyArrowConfigExceptionInvalidJsonFile : public std::exception {
            public:
                TinyArrowConfigExceptionInvalidJsonFile(const std::string& fileName, const char *jsonMsg)
                {
                    std::string msg = "Undertale::Config::TinyArrowConfig Trying to construct from an invalid json file '" + fileName + "'.\nError: '";

                    this->_concat = msg + jsonMsg + "'.";
                };
                const char *what() const noexcept
                {
                    return this->_concat.c_str();
                };

            private:
                std::string _concat;
        };

        class FloatRect {
            public:
                FloatRect(void)
                    : _top(0), _left(0), _width(0), _height(0) {}
                FloatRect(float top, float left, float width, float height)
                    : _top(top), _left(left), _width(width), _height(height) {}

                bool collidesWith(const FloatRect& other) const noexcept {
                    return !(
                    _left > other._left + other._width ||
                    _left + _width < other._left ||
                    _top > other._top + other._height ||
                    _top + _height < other._top
                    );
                }

            public:
                float _top;
                float _left;
                float _width;
                float _height;
        };

        class TinyArrowSpec {
            public:
                enum Direction {
                    UP,
                    DOWN,
                    LEFT,
                    RIGHT
                };

                TinyArrowSpec(void) = default;
                TinyArrowSpec(Direction direction, float velocity, float startTime, const Rengine::Graphics::vector2D<float>& pos);
                ~TinyArrowSpec(void) = default;

                void draw(void) const noexcept;
                void update(float delta);
                bool collide(const FloatRect& rect) const;
                Direction getDir(void) const;
                float getTime(void) const;

            private:
                Direction _direction;
                float _velocity;
                std::shared_ptr<Rengine::Graphics::ASprite> _sprite;
                float _startTime;
                FloatRect _rect;
        };

        class TinyArrowConfig {
            public:
                TinyArrowConfig(const std::string& jsonPath);
                ~TinyArrowConfig(void) = default;

                void parseTinyArrow(const nlohmann::json& j);
                bool checkCollision(const FloatRect& bounds);
                void draw(void);
                void update(void);
                bool attackOver(void);

            private:
                void createPlayer(void);

                std::vector<std::unique_ptr<TinyArrowSpec>> _tinyArrows;
                std::vector<std::unique_ptr<TinyArrowSpec>> _availableArrows;

                std::shared_ptr<Rengine::Graphics::ASprite> _playerDef;
                FloatRect _defRect;
                std::shared_ptr<Rengine::Graphics::ASprite> _player;
                FloatRect _playerRect;
                sf::Clock _gameClock;
                sf::Time _arrowTime;
                sf::Time _inputTime;
                sf::Time _overallTime;
                TinyArrowSpec::Direction _dir = TinyArrowSpec::UP;
        };
    }
}

#endif