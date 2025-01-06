#include "JsonManager.hpp"

bsf::JsonManager::JsonManager() {
}

bsf::JsonManager::~JsonManager() {
}

nlohmann::json &bsf::JsonManager::saveAndGet(const std::string& path) {
    if (jsons.find(path) != jsons.end())
        return jsons[path];

    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);
    
    nlohmann::json json;
    try {
        file >> json;
    } catch (nlohmann::json::parse_error& e) {
        throw std::runtime_error("Could not parse json: " + path);
    }
    file.close();

    jsons[path] = json;
    return jsons[path];
}