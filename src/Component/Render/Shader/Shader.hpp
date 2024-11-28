#pragma once
#include <SFML/Graphics.hpp>

class Shader : public sf::Shader {
    public:
        Shader(const std::string& filename);
        ~Shader();
        
};
