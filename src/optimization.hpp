#pragma once

#include <optional>
#include <string>
#include <vector>

#include "generation.hpp"

class Optimizer {
public:
    Optimizer (std::vector<Pattern>& patterns);

    std::vector<Pattern> optimize();
private:
    std::vector<Pattern> m_patterns;
};