#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace bsf{
    class RenderWindow;
}

namespace bsf{
    class WindowParser {
        public:
            WindowParser();
            ~WindowParser();
            void parse(const std::string& path);
            void setWindow(bsf::RenderWindow& window);

        private:
            nlohmann::json json;
            std::ifstream file;
            bsf::RenderWindow *window;

            void parseTitle();
            void parseResolution();
            void parseFramerateLimit();
            void parseBackgroundColor();
            void parseIcon();
            void parseScreenMode();
            void parseVsync();
            void parseCursor();
            void parseView();
            void parseInput();
    };
}