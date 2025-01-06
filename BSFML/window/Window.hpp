#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "WindowParser.hpp"
#include "../RessourceManager/RessourceManager.hpp"
#include "../Texture/Texture.hpp"
#include "../Vector/Vector2.hpp"

namespace bsf {
    class RenderWindow : public sf::RenderWindow {
        public:
            RenderWindow(const std::string& configfile="configs/window.json");
            ~RenderWindow() = default;
            
            std::string &getTitle();
            void setTitle(const std::string& title);
            void setBackgroundColor(const sf::Color& color);
            void clear();
            void clear(const sf::Color& color);
            bool pollEvent(sf::Event& event);

            float getDeltaTime();

            bsf::RessourceManager<bsf::Texture> &getTextureManager();
            bsf::RessourceManager<sf::Font> &getFontManager();
            bsf::RessourceManager<sf::SoundBuffer> &getSoundManager();
            bsf::RessourceManager<sf::Music> &getMusicManager();
            bsf::RessourceManager<sf::Shader> &getShaderManager();

            bsf::Vector2 getMousePositionLocal();
            bsf::Vector2 getMousePositionGlobal();

            sf::Keyboard::Key closeInput;
            sf::Keyboard::Key fullscreenInput;

        private:
            std::string title;
            sf::Color backgroundColor;
            bsf::WindowParser parser;
            sf::Clock clock;

            bsf::RessourceManager<bsf::Texture> textureManager;
            bsf::RessourceManager<sf::Font> fontManager;
            bsf::RessourceManager<sf::SoundBuffer> soundManager;
            bsf::RessourceManager<sf::Music> musicManager;
            bsf::RessourceManager<sf::Shader> shaderManager;
    };
};