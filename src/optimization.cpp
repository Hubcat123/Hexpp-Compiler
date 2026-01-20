#include "optimization.hpp"

Optimizer::Optimizer (std::vector<Pattern>& patterns)
    :m_patterns(patterns)
{ }

std::vector<Pattern> Optimizer::optimize()
{
    return m_patterns;
}