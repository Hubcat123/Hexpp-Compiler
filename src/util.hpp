#pragma once

#include <string>

void compilation_error(const std::string message, size_t line);
void compilation_warning(const std::string message, size_t line);