#pragma once

#include <optional>
#include <string>
#include <vector>

#include "generation.hpp"

class Optimizer {
public:
    Optimizer (std::vector<Pattern> patterns);

    std::vector<Pattern> optimize();
private:
    std::vector<Pattern> m_output = std::vector<Pattern>();
    std::vector<Pattern> m_patterns;
    size_t m_index = 0;

    void add_pattern(PatternType type, std::optional<std::string> value = {});
    void add_pattern(Pattern pattern);

    bool is_valid_vector_constant(std::string x, std::string y, std::string z);
    // Currently only supports num ops
    bool is_non_division_binary_op(PatternType type);

    std::optional<Pattern> peek(int offset = 0);
    Pattern consume();
    void consume(int amount);
};