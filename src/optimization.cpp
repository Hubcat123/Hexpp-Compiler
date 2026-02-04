#include "optimization.hpp"

#include "util.hpp"

#define no_opt() no_optimization = true;add_pattern(consume());

Optimizer::Optimizer (std::vector<Pattern> patterns)
    :m_patterns(std::vector<Pattern>(patterns))
{ }

std::vector<Pattern> Optimizer::optimize()
{
    // Loop until no more optimizations are found
    while (true)
    {
        bool found_optimizations = false;

        // TODO: Boolean optimizations, do boolean ops between boolean literals and when doing boolean logic that can be solved at compile time
        // Loop over all patterns
        while (peek().has_value())
        {
            bool no_optimization = false;

            switch (peek().value().type)
            {
            case PatternType::jesters_gambit:
                // Double swap, do nothing
                if (peek(1).has_value() && peek(1).value().type == PatternType::jesters_gambit)
                {
                    consume(2);
                }
                else
                {
                    no_opt();
                }
                break;
            case PatternType::gemini_decomposition:
                // Copying value that will be immediately erased, don't copy
                if (peek(1).has_value() && peek(1).value().type == PatternType::bookkeepers_gambit && peek(1).value().value.value().back() == 'v')
                {
                    consume();
                    // Remove last char
                    std::string new_val = consume().value.value();
                    new_val.pop_back();
                    add_pattern(PatternType::bookkeepers_gambit, new_val);
                }
                else
                {
                    no_opt();
                }
                break;
            case PatternType::fishermans_gambit_II:
                // Copying value that will be immediately erased, don't copy
                if (peek(1).has_value() && peek(1).value().type == PatternType::bookkeepers_gambit && peek(1).value().value.value().back() == 'v')
                {
                    consume();
                    add_pattern(PatternType::fishermans_gambit);
                    // Remove last char
                    std::string new_val = consume().value.value();
                    new_val.pop_back();
                    add_pattern(PatternType::bookkeepers_gambit, new_val);
                }
                else
                {
                    no_opt();
                }
                break;
            case PatternType::introspection:
                // List of pattern lits
                if (peek(1).has_value() && peek(1).value().type == PatternType::pattern_lit &&
                    peek(2).has_value() && peek(2).value().type == PatternType::retrospection &&
                    peek(3).has_value() && peek(3).value().type == PatternType::flocks_disintegration)
                {
                    // Keep looking for more pattern lits
                    int num_patterns = 1;
                    while (peek(4 * num_patterns + 0).has_value() && peek(4 * num_patterns + 0).value().type == PatternType::introspection &&
                           peek(4 * num_patterns + 1).has_value() && peek(4 * num_patterns + 1).value().type == PatternType::pattern_lit &&
                           peek(4 * num_patterns + 2).has_value() && peek(4 * num_patterns + 2).value().type == PatternType::retrospection &&
                           peek(4 * num_patterns + 3).has_value() && peek(4 * num_patterns + 3).value().type == PatternType::flocks_disintegration)
                    {
                        ++num_patterns;
                    }

                    // Check for flocks gambit with correct number of patterns
                    if (peek(4 * num_patterns + 0).has_value() && peek(4 * num_patterns + 0).value().type == PatternType::numerical_reflection &&
                        peek(4 * num_patterns + 0).value().value.value() == std::to_string(num_patterns) &&
                        peek(4 * num_patterns + 1).has_value() && peek(4 * num_patterns + 1).value().type == PatternType::flocks_gambit)
                    {
                        // build combined pattern list
                        add_pattern(PatternType::introspection);
                        for (int i = 0; i < num_patterns; ++i)
                        {
                            add_pattern(PatternType::pattern_lit, peek(4 * i + 1).value().value);
                        }
                        add_pattern(PatternType::retrospection);

                        // Consume patterns
                        consume(4 * num_patterns + 2);
                    }
                    else
                    {
                        no_opt();
                    }
                }
                else
                {
                    no_opt();
                }
                break;
            case PatternType::bookkeepers_gambit:
                // Dud bookkeepers, do nothing
                if (peek().value().value.value().find('v') == std::string::npos)
                {
                    consume();
                }
                // Double bookkeepers, combine them
                else if (peek(1).has_value() && peek(1).value().type == PatternType::bookkeepers_gambit)
                {
                    // Grab values of both bookkeepers to combine
                    std::string first_val = consume().value.value();
                    std::string second_val = consume().value.value();

                    // Loop over first value from left to right
                    int second_ind = second_val.length() - 1;
                    for (int i = first_val.length() - 1; i >= 0 && second_ind >= 0; --i)
                    {
                        // If first value has a -, replace with current char from second value
                        if (first_val[i] == '-')
                        {
                            first_val[i] = second_val[second_ind];
                            --second_ind;
                        }
                    }

                    // If second val still has characters, make them the left side of the new val
                    std::string new_val = second_val.substr(0, second_ind + 1) + first_val;
                    add_pattern(PatternType::bookkeepers_gambit, new_val);
                }
                else
                {
                    no_opt();
                }
                break;
            // Check for operators which output bools
            case PatternType::equality_distillation:
            case PatternType::inequality_distillation:
            case PatternType::maximus_distillation:
            case PatternType::maximus_distillation_II:
            case PatternType::minimus_distillation:
            case PatternType::minimus_distillation_II:
            case PatternType::conjunction_distillation:
            case PatternType::disjunction_distillation:
                // Top of stack is already a bool, no need to purify
                if (peek(1).has_value() && peek(1).value().type == PatternType::augurs_purification)
                {
                    add_pattern(consume());
                    consume();
                }
                else
                {
                    no_opt();
                }
                break;
            case PatternType::numerical_reflection:
                // Fishing down by 2, rotate down instead
                if (peek().value().value.value() == "-2" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::fishermans_gambit)
                {
                    consume(2);
                    add_pattern(PatternType::rotation_gambit_II);
                }
                // Fishing down by 1, swap instead
                else if (peek().value().value.value() == "-1" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::fishermans_gambit)
                {
                    consume(2);
                    add_pattern(PatternType::jesters_gambit);
                }
                // Fishing up top element, do nothing
                else if (peek().value().value.value() == "0" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::fishermans_gambit)
                {
                    consume(2);
                }
                // Fishing up copy of top element, dupe instead
                else if (peek().value().value.value() == "0" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::fishermans_gambit_II)
                {
                    consume(2);
                    add_pattern(PatternType::gemini_decomposition);
                }
                // Fishing up second-from-top element, swap instead
                else if (peek().value().value.value() == "1" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::fishermans_gambit)
                {
                    consume(2);
                    add_pattern(PatternType::jesters_gambit);
                }
                // Fishing up copy of second-from-top element, dupe instead
                else if (peek().value().value.value() == "1" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::fishermans_gambit_II)
                {
                    consume(2);
                    add_pattern(PatternType::prospectors_gambit);
                }
                // Fishing up copy of third-from-top element, rotate down
                else if (peek().value().value.value() == "2" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::fishermans_gambit)
                {
                    consume(2);
                    add_pattern(PatternType::rotation_gambit);
                }
                // Zero element flock's, vacant instead
                else if (peek().value().value.value() == "0" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::flocks_gambit)
                {
                    consume(2);
                    add_pattern(PatternType::vacant_reflection);
                }
                // Single element flock's, single's instead
                else if (peek().value().value.value() == "1" && peek(1).has_value() &&
                    peek(1).value().type == PatternType::flocks_gambit)
                {
                    consume(2);
                    add_pattern(PatternType::singles_purification);
                }
                // Vector constant optimizations
                else if (peek(1).has_value() && peek(1).value().type == PatternType::numerical_reflection &&
                    peek(2).has_value() && peek(2).value().type == PatternType::numerical_reflection &&
                    peek(3).has_value() && peek(3).value().type == PatternType::vector_exaltation &&
                    is_valid_vector_constant(peek().value().value.value(), peek(1).value().value.value(), peek(2).value().value.value()))
                {
                    if (peek().value().value.value() != "0")
                    {
                        if (peek().value().value.value() == "1")
                        {
                            add_pattern(PatternType::vector_reflection_PX);
                        }
                        else
                        {
                            add_pattern(PatternType::vector_reflection_NX);
                        }
                    }
                    else if (peek(1).value().value.value() != "0")
                    {
                        if (peek(1).value().value.value() == "1")
                        {
                            add_pattern(PatternType::vector_reflection_PY);
                        }
                        else
                        {
                            add_pattern(PatternType::vector_reflection_NY);
                        }
                    }
                    else if (peek(2).value().value.value() != "0")
                    {
                        if (peek(2).value().value.value() == "1")
                        {
                            add_pattern(PatternType::vector_reflection_PZ);
                        }
                        else
                        {
                            add_pattern(PatternType::vector_reflection_NZ);
                        }
                    }
                    else
                    {
                        add_pattern(PatternType::vector_reflection_zero);
                    }

                    consume(4);
                }
                // Compile-time math
                else if (peek(1).has_value() && peek(1).value().type == PatternType::numerical_reflection &&
                    peek(2).has_value() && is_non_division_binary_op(peek(2).value().type))
                {
                    float lhs = std::stof(consume().value.value());
                    float rhs = std::stof(consume().value.value());

                    float result;

                    // Perform operation
                    switch (consume().type)
                    {
                    case PatternType::additive_distillation:
                        result = lhs + rhs;
                        break;
                    case PatternType::subtractive_distillation:
                        result = lhs - rhs;
                        break;
                    case PatternType::multiplicative_distillation:
                        result = lhs * rhs;
                        break;
                    }

                    // Maintain lack of decimal place if possible
                    if (result == (int)result)
                    {
                        add_pattern(PatternType::numerical_reflection, std::to_string((int)result));
                    }
                    else
                    {
                        add_pattern(PatternType::numerical_reflection, std::to_string(result));
                    }
                }
                else
                {
                    no_opt();
                }
                break;
            // No optimizations to be found, continue
            default:
                no_opt();
                break;
            }

            found_optimizations = found_optimizations || !no_optimization;
        }

        if (!found_optimizations)
        {
            return m_output;
        }

        m_patterns = std::vector<Pattern>(m_output);
        m_index = 0;
        m_output.clear();
    }
}

void Optimizer::add_pattern(PatternType type, std::optional<std::string> value)
{
    m_output.push_back(Pattern{.type = type, .value = value});
}

void Optimizer::add_pattern(Pattern pattern)
{
    m_output.push_back(pattern);
}

bool Optimizer::is_valid_vector_constant(std::string x, std::string y, std::string z)
{
    int num_zeros = (x == "0") + (y == "0") + (z == "0");

    if (num_zeros >= 3)
    {
        return true;
    }

    if (num_zeros < 2)
    {
        return false;
    }

    int num_ones = (x == "1") + (y == "1") + (z == "1");

    if (num_ones == 1)
    {
        return true;
    }

    int num_neg_ones = (x == "-1") + (y == "-1") + (z == "-1");

    if (num_neg_ones == 1)
    {
        return true;
    }

    return false;
}

bool Optimizer::is_non_division_binary_op(PatternType type)
{
    switch (type)
    {
    case PatternType::additive_distillation:
    case PatternType::subtractive_distillation:
    case PatternType::multiplicative_distillation:
        return true;
        break;
    default:
        return false;
        break;
    }
}

std::optional<Pattern> Optimizer::peek(int offset)
{
    if (m_index + offset >= m_patterns.size())
    {
        return {};
    }

    return m_patterns.at(m_index + offset);
}

Pattern Optimizer::consume()
{
    return m_patterns.at(m_index++);
}

void Optimizer::consume(int amount)
{
    m_index += amount;
}