#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <string>

class Shader {
        private:
            std::shared_ptr<sf::Shader> shader;
            std::unordered_map<std::string, float> uniforms;
        public:
            Shader(const std::string& filename, std::unordered_map<std::string, float> uniforms);
            ~Shader();
            sf::Shader& getShader();
            void setUniform(const std::string& name, float value);
            std::unordered_map<std::string, float> &getUniforms();
};

