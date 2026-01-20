#pragma once

#include "optimization.hpp"

class Assembler {
public:
    Assembler(std::vector<Pattern> patterns);

    std::string assemble();
private:
    const std::string m_pattern_strings[PatternType::num_patterns] = {
        "Additive Distillation", "Alidade's Purification", "Archer's Distillation", "Architect's Distillation", "Augur's Exaltation", "Augur's Purification", "Bookkeeper's Gambit", "Break Block",
        "Charon's Gambit", "Compass' Purification", "Compass' Purification II", "Conjunction Distillation", "Conjure Light", "Conjure Water", "Dioscuri Gambit", "Disjunction Distillation",
        "Division Distillation", "Equality Distillation", "False Reflection", "Fisherman's Gambit", "Fisherman's Gambit II", "Flock's Disintegration", "Flock's Gambit", "Flock's Reflection",
        "Gemini Decomposition", "Hermes' Gambit", "Huginn's Gambit", "Inequality Distillation", "Integration Distillation", "{", "Iris' Gambit", "Jester's Gambit", "Length Purification",
        "Locator's Distillation", "Maximus Distillation", "Maximus Distillation II", "Mind's Reflection", "Minimus Distillation", "Minimus Distillation II", "Modulus Distillation",
        "Multiplicative Distillation", "Muninn's Reflection", "Negation Purification", "Nullary Reflection", "Numerical Reflection", "Power Distillation", "}", "Reveal", "Rotation Gambit",
        "Rotation Gambit II", "Single's Purification", "Selection Distillation", "Subtractive Distillation", "Surgeon's Exaltation", "Thoth's Gambit", "True Reflection", "Vacant Reflection",
        "Vector Exaltation"
    };

    std::vector<Pattern> m_patterns;

    int m_indent_level = 0;
};