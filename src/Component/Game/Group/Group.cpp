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

bool Group::has(std::string group) {
    for (uint32_t i = 0; i < this->groups.size(); i++) {
        if (this->groups[i] == group) {
            return true;
        }
    }
    return false;
}

bool Group::has(std::vector<std::string> groups) {
    for (uint32_t i = 0; i < groups.size(); i++) {
        if (!this->has(groups[i])) {
            return false;
        }
    }
    return true;
}

std::vector<std::string>& Group::getGroups() {
    return this->groups;
}
