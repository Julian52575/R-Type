#include "./Shader.hpp"
#include <memory>
#include <string>
#include <unordered_map>

Shader:: Shader(const std::string& filename) {
    shader = std::make_shared<sf::Shader>();
    if (!((*shader).loadFromFile(filename, sf::Shader::Fragment))) {
        throw std::runtime_error("Could not load shader");
    }
}

Shader::~Shader() {}

sf::Shader& Shader::getShader() {
    return *shader;
}

void Shader::setUniform(const std::string& name, float value) {
    shader->setUniform(name, value);
}


