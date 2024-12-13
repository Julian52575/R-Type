#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <string>

class Shader {
        private:
            std::shared_ptr<sf::Shader> shader;
        public:
            Shader(const std::string& filename);
            ~Shader();
            sf::Shader& getShader();
            void setUniform(const std::string& name, float value);
};

