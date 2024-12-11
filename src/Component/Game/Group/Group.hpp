#pragma once
#include <vector>
#include <string>

class Group {
    public:
        Group();
        explicit Group(std::string group);
        explicit Group(std::vector<std::string> groups);
        ~Group();
        void add(std::string group);
        void remove(std::string group);
        void clear();
    private:
        std::vector<std::string> groups;
};