#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>

namespace bsf{
    template <typename Ressource>
    class RessourceManager{
        private:
            std::unordered_map<std::string,std::unique_ptr<Ressource>> ressources;

        public:
            RessourceManager() = default;
            ~RessourceManager() = default;

            Ressource &saveAndGet(const std::string& filename);
            void deleteRessource(const std::string& ressource);
            void deleteRessource(Ressource &ressource);
    };
}

template <typename Ressource>
Ressource &bsf::RessourceManager<Ressource>::saveAndGet(const std::string& filename) {
    if (this->ressources.find(filename) != this->ressources.end()) {
        return *this->ressources.at(filename);
    }

    Ressource ressource;
    if (ressource.loadFromFile(filename)) {
        std::unique_ptr<Ressource> ressourcePtr = std::make_unique<Ressource>(ressource);
        this->ressources[filename] = std::move(ressourcePtr);
        return *this->ressources.at(filename);
    }
    throw std::runtime_error("Could not load ressource: " + filename);
}

template <typename Ressource>
void bsf::RessourceManager<Ressource>::deleteRessource(const std::string& ressource) {
    if (this->ressources.find(ressource) == this->ressources.end()) {
        throw std::runtime_error("Could not find ressource: " + ressource);
    }
    this->ressources.erase(ressource);
}

template <typename Ressource>
void bsf::RessourceManager<Ressource>::deleteRessource(Ressource &ressource) {
    for (auto it = this->ressources.begin(); it != this->ressources.end(); ++it) {
        if (it->second.get() == &ressource) {
            this->ressources.erase(it);
            return;
        }
    }
    throw std::runtime_error("Could not find ressource: " + ressource);
}