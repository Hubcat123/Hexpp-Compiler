#include "assembler.hpp"

Assembler::Assembler(std::vector<Pattern> patterns, bool use_hexagon_alternatives)
    :m_patterns(patterns), m_using_hexagon(use_hexagon_alternatives)
{ } 

std::string Assembler::assemble()
{
    // Create stream to populate with code
    std::stringstream stream;

    // Loop over patterns and construct output
    for (Pattern p : m_patterns)
    {
        // Dedent if retrospection
        if (p.type == PatternType::retrospection)
        {
            --m_indent_level;
        }

        // If pattern is a pattern literal, directly output value
        if (p.type == PatternType::embedded_iota)
        {
            stream << std::string(m_indent_level, '\t') << p.value.value() << '\n';
        }
        // If we're using hexagon alternatives and one exists, use it
        else if (m_using_hexagon && m_hexagon_alternatives.find(p.type) != m_hexagon_alternatives.end())
        {
            stream << std::string(m_indent_level, '\t') << m_hexagon_alternatives.at(p.type) << (p.value.has_value() ? (std::string(": ") + p.value.value()) : "") << '\n';
        }
        else
        {
            stream << std::string(m_indent_level, '\t') << m_pattern_strings[p.type] << (p.value.has_value() ? (std::string(": ") + p.value.value()) : "") << '\n';
        }

        // Indent if introspection
        if (p.type == PatternType::introspection)
        {
            ++m_indent_level;
        }
    }

    // Return string
    return stream.str();
}