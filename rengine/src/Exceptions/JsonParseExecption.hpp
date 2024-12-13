#pragma once
#include <stdexcept>
#include <string>

class JsonParseException : public std::runtime_error {
    public:
        explicit JsonParseException(const std::string& message) : std::runtime_error(message) {}
};
