#include "./Group.hpp"
#include <string>
#include <cstdint>

Group::Group() {}

Group::Group(std::string group) {
    this->groups.push_back(group);
}

Group::Group(std::vector<std::string> groups) {
    this->groups = groups;
}

Group::~Group() {}

void Group::add(std::string group) {
    this->groups.push_back(group);
}

void Group::remove(std::string group) {
    for (uint32_t i = 0; i < this->groups.size(); i++) {
        if (this->groups[i] == group) {
            this->groups.erase(this->groups.begin() + i);
            return;
        }
    }
}

void Group::clear() {
    this->groups.clear();
}

