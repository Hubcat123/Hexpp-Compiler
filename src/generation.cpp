#include "generation.hpp"

#include <sstream>

Generator::Generator(const NodePrint& root)
    :m_root(root)
{ }

std::string Generator::generate() const
{
    std::stringstream output;

    output << "Numerical Reflection: " << m_root.expr.num_lit.value.value() << '\n';
    output << "Reveal\n";
    output << "Bookkeeper's Gambit: v\n";

    return output.str();
}