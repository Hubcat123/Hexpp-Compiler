#pragma once

#include "tokenization.hpp"

struct NodeExpr {
    Token num_lit;
};

struct NodePrint {
    NodeExpr expr;
};

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);

    std::optional<NodePrint> parse();
private:
    std::optional<NodeExpr> parse_expr();

    std::optional<Token> peek(int ahead = 0) const;
    Token consume();

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
};