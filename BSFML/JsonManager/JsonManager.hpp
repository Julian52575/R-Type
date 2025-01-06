#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>

namespace bsf{
    class JsonManager {
        public:
            JsonManager();
            ~JsonManager();
            nlohmann::json &saveAndGet(const std::string& path);
            
            template <typename T>
            T getData(const std::string& path, const std::string& key);

        private:
            std::unordered_map<std::string,nlohmann::json> jsons;
    };
}

template <typename T>
T bsf::JsonManager::getData(const std::string& path, const std::string& key) {
    nlohmann::json &json = saveAndGet(path);
    std::istringstream iss(key);
    std::string segment;
    nlohmann::json *temp = &json;
    
    while (std::getline(iss, segment, '.')) {
        size_t bracket_pos = segment.find('[');
        if (bracket_pos != std::string::npos) {
            std::string key_part = segment.substr(0, bracket_pos);
            size_t closing_bracket_pos = segment.find(']');
            if (closing_bracket_pos == std::string::npos)
                throw std::runtime_error("Invalid bracket notation in key: " + segment);
            int index = -1;
            try {
                index = std::stoi(segment.substr(bracket_pos + 1, closing_bracket_pos - bracket_pos - 1));
            } catch (std::invalid_argument& e){
                throw std::runtime_error("Invalid index in key: " + segment);
            }
            if (!temp->contains(key_part))
                throw std::runtime_error("Could not find key: " + key_part);
            temp = &(*temp)[key_part];
            if (!temp->is_array() || index >= temp->size())
                throw std::out_of_range("Index out of bounds for key: " + key_part);
            temp = &(*temp)[index];
        } else {
            if (!temp->contains(segment))
                throw std::runtime_error("Could not find key: " + key + " ( " + segment + " ) in " + path);
            temp = &(*temp)[segment];
        }
    }
    try {
        T value = temp->get<T>();
        return value;
    }
    catch (nlohmann::json::exception& e) {
        std::string message = e.what();
        std::regex regex("type must be (\\w+), but is (\\w+)");
        std::smatch matches;

        if (std::regex_search(message, matches, regex)) {
            std::string expectedType = matches[2].str();
            std::string actualType = matches[1].str();
            throw std::runtime_error(expectedType + " expected, but " + actualType + " is given for key: " + key);
        }
        throw std::runtime_error("Could not get data: " + key + " in " + path);
    }
}
