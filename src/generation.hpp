#pragma once

#include "parser.hpp"

class Generator {
public:
    Generator(const NodePrint& root);

    std::string generate() const;
private:
    const NodePrint m_root;
};