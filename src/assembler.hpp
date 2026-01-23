#pragma once

#include "optimization.hpp"

class Assembler {
public:
    Assembler(std::vector<Pattern> patterns);

    std::string assemble();
private:
    const std::string m_pattern_strings[PatternType::num_patterns] = {
        "Akasha's Distillation", "Akasha's Gambit", "Additive Distillation", "Alidade's Purification", "Anchorite's Flight", "Archer's Distillation",
        "Architect's Distillation", "Arc's Reflection", "Augur's Exaltation", "Augur's Purification", "Auditor's Purification", "Auditor's Reflection",
        "Assessor's Purification", "Assessor's Reflection", "Aviator's Purification", "Black Sun's Nadir", "Black Sun's Zenith", "Blue Sun's Nadir",
        "Blue Sun's Zenith", "Bookkeeper's Gambit", "Break Block", "Charon's Gambit", "Chronicler's Gambit", "Chronicler's Purification", "Circle's Reflection",
        "Compass' Purification", "Compass' Purification II", "Conjunction Distillation", "Conjure Light", "Consideration", "Cosine Purification", "Create Water",
        "Derivation Decomposition", "Dioscuri Gambit", "Disjunction Distillation", "Division Distillation", "Dispel Rain", "Euler's Reflection",
        "Equality Distillation", "Evanition", "Explosion", "False Reflection", "Fireball", "Floor Purification", "Fisherman's Gambit", "Fisherman's Gambit II",
        "Flock's Disintegration", "Flock's Gambit", "Flock's Reflection", "Gemini Decomposition", "Gemini Gambit", "Greater Fold Reflection", "Hermes' Gambit",
        "Huginn's Gambit", "Inequality Distillation", "Integration Distillation", "Inverse Cosine Purification", "Inverse Sine Purification",
        "Inverse Tangent Distillation", "Introspection", "Iris' Gambit", "Jester's Gambit", "Length Purification", "Locator's Distillation", "Lodestone Reflection",
        "Logarithmic Distillation", "Make Note", "Maximus Distillation", "Maximus Distillation II", "Mind's Reflection", "Minimus Distillation",
        "Minimus Distillation II", "Modulus Distillation", "Multiplicative Distillation", "Muninn's Reflection", "Negation Purification", "Nullary Reflection",
        "Numerical Reflection", "Pace Purification", "Power Distillation", "Prospector's Gambit", "Retrospection", "Reveal", "Rotation Gambit", "Rotation Gambit II",
        "Scout's Distillation", "Selection Distillation", "Selection Exaltation", "Speaker's Decomposition", "Speaker's Distillation", "Single's Purification", "Stadiometer's Purification",
        "Subtractive Distillation", "Sine Purification", "Swindler's Gambit", "Summon Lightning", "Summon Rain", "Surgeon's Exaltation", "Thanatos' Reflection",
        "Thoth's Gambit", "True Reflection", "Vacant Reflection", "Vector Disintegration", "Vector Exaltation", "Vector Reflection -X", "Vector Reflection -Y",
        "Vector Reflection -Z", "Vector Reflection +X", "Vector Reflection +Y", "Vector Reflection +Z", "Vector Reflection Zero", "Wayfarer's Flight",
        "Waystone Reflection", "White Sun's Nadir", "White Sun's Zenith"
    };

    std::vector<Pattern> m_patterns;

    int m_indent_level = 0;
};