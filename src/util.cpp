#include "util.hpp"

#include <iostream>

void compilation_error(const std::string message, size_t line)
{
    if (line > 0)
    {
        std::cout << "Hex++ Compilation Error: " << message << " on line " << line << std::endl;
    }
    else
    {
        std::cout << "Hex++ Compilation Error: " << message << std::endl;
    }
    exit(EXIT_FAILURE);
}

void compilation_warning(const std::string message, size_t line)
{
    if (line > 0)
    {
        std::cout << "Hex++ Compilation Warning: " << message << " on line " << line << std::endl;
    }
    else
    {
        std::cout << "Hex++ Compilation Warning: " << message << std::endl;
    }
}