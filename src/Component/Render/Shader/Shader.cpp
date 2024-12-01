#include "Shader.hpp"

Shader:: Shader(const std::string& filename,std::unordered_map<std::string, float> uniforms = {}) : uniforms(uniforms) {
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
    uniforms[name] = value;
    shader->setUniform(name, value);
}

std::unordered_map<std::string, float> &Shader::getUniforms() {
    return uniforms;
}