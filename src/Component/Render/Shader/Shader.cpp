#include "Shader.hpp"

Shader::Shader(const std::string& filename) {
    if (!loadFromFile(filename, sf::Shader::Fragment)) {
        throw std::runtime_error("Could not load shader");
    }
}

Shader::~Shader() {}