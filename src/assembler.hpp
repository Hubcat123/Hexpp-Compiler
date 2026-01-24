#pragma once

#include <unordered_map>

#include "optimization.hpp"

class Assembler {
public:
    Assembler(std::vector<Pattern> patterns, bool use_hexagon_alternatives);

    std::string assemble();
private:
    const std::string m_pattern_strings[PatternType::num_patterns] = {
        "Akasha's Distillation", "Akasha's Gambit", "Additive Distillation", "Alidade's Purification", "Anchorite's Flight", "Archer's Distillation",
        "Architect's Distillation", "Arc's Reflection", "Augur's Exaltation", "Augur's Purification", "Auditor's Purification", "Auditor's Reflection",
        "Assessor's Purification", "Assessor's Reflection", "Aviator's Purification", "Axial Purification",

        "Black Sun's Nadir", "Black Sun's Zenith", "Blue Sun's Nadir", "Blue Sun's Zenith", "Bookkeeper's Gambit", "Break Block",

        "Ceiling Purification", "Charon's Gambit", "Chronicler's Gambit", "Chronicler's Purification", "Circle's Reflection", "Compass' Purification",
        "Compass' Purification II", "Conjunction Distillation", "Conjure Light", "Consideration", "Cosine Purification", "Create Water",

        "Derivation Decomposition", "Dioscuri Gambit", "Disjunction Distillation", "Division Distillation", "Dispel Rain",

        "Entropy Reflection", "Euler's Reflection", "Entity Purification", "Entity Purification: Animal", "Entity Purification: Item", "Entity Purification: Living",
        "Entity Purification: Monster", "Entity Purification: Player", "Equality Distillation", "Evanition", "Excisors Distillation", "Exclusion Distillation",
        "Explosion",

        "False Reflection", "Fireball", "Fisherman's Gambit", "Fisherman's Gambit II", "Flock's Disintegration", "Flock's Gambit", "Flock's Reflection",
        "Floor Purification",

        "Gemini Decomposition", "Gemini Gambit", "Greater Fold Reflection",

        "Hermes' Gambit", "Huginn's Gambit",

        "Inequality Distillation", "Integration Distillation", "Inverse Cosine Purification", "Inverse Sine Purification", "Inverse Tangent Distillation",
        "Inverse Tangent Purification", "{", "Iris' Gambit",

        "Jester's Gambit",

        "Length Purification", "Lesser Fold Reflection", "Locator's Distillation", "Lodestone Reflection", "Logarithmic Distillation",

        "Make Note", "Maximus Distillation", "Maximus Distillation II", "Mind's Reflection", "Minimus Distillation", "Minimus Distillation II",
        "Modulus Distillation", "Multiplicative Distillation", "Muninn's Reflection",

        "Negation Purification", "Nullary Reflection", "Numerical Reflection",

        "Pace Purification", "Power Distillation", "Prospector's Gambit",

        "Retrograde Purification", "}", "Reveal", "Rotation Gambit", "Rotation Gambit II",

        "Scout's Distillation", "Scribe's Gambit", "Scribe's Reflection", "Selection Distillation", "Selection Exaltation", "Speaker's Decomposition",
        "Speaker's Distillation", "Single's Purification", "Stadiometer's Purification", "Subtractive Distillation", "Sine Purification", "Swindler's Gambit",
        "Summon Lightning", "Summon Rain", "Surgeon's Exaltation",

        "Tangent Purification", "Thanatos' Reflection", "Thoth's Gambit", "True Reflection",

        "Uniqueness Purification",

        "Vacant Reflection", "Vector Disintegration", "Vector Exaltation", "Vector Reflection -X", "Vector Reflection -Y", "Vector Reflection -Z",
        "Vector Reflection +X", "Vector Reflection +Y", "Vector Reflection +Z", "Vector Reflection Zero",

        "Wayfarer's Flight", "Waystone Reflection", "White Sun's Nadir", "White Sun's Zenith",
        
        "Zone Distillation: Animal", "Zone Distillation: Any", "Zone Distillation: Item", "Zone Distillation: Living", "Zone Distillation: Monster",
        "Zone Distillation: Non-Animal", "Zone Distillation: Non-Item", "Zone Distillation: Non-Living", "Zone Distillation: Non-Monster",
        "Zone Distillation: Non-Player", "Zone Distillation: Player"
    };

    const std::unordered_map<PatternType, std::string> m_hexagon_alternatives = {
        {PatternType::derivation_decomposition, "Combination Distillation"},
    };

    bool m_using_hexagon;

    std::vector<Pattern> m_patterns;

    int m_indent_level = 0;
};