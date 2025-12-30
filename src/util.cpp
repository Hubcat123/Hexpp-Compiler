#include "util.hpp"

#include <iostream>

void compilation_error(const std::string message)
{
    std::cout << "Hex++ Compilation Error: " << message << std::endl;
    exit(EXIT_FAILURE);
}