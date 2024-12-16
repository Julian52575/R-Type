#include "./include/utils.hpp"

std::string find_entity_path(uint16_t id)
{
    std::ifstream file("assets/entities/entities.json");
    nlohmann::json json;
    file >> json;
    if (json.find(std::to_string(id)) != json.end())
    {
        return json[std::to_string(id)];
    }
    return "";
}
