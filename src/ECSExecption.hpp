#pragma once
#include <stdexcept>
#include <string>

class ECSException : public std::runtime_error {
    public:
        explicit ECSException(const std::string& message) : std::runtime_error(message) {}
};
