#include "parser.hpp"

#include "util.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    :m_tokens(tokens)
{ }

std::optional<NodePrint> Parser::parse()
{
    NodePrint root = {};

    while (peek().has_value())
    {
        if (peek().value().type == TokenType::print)
        {
            consume();
            if (std::optional<NodeExpr> node_expr = parse_expr())
            {
                root = NodePrint {.expr = node_expr.value()};
            }
            else
            {
                compilation_error("Invalid expression");
            }

            if (peek().has_value() && peek().value().type == TokenType::semi)
            {
                consume();
            }
            else
            {
                compilation_error("Expected ';'");
            }
        }
    }

    m_index = 0;
    return root;
}

std::optional<NodeExpr> Parser::parse_expr()
{
    if (peek().has_value() && peek().value().type == TokenType::num_lit)
    {
        return NodeExpr{.num_lit = consume()};
    }

    return {};
}

std::optional<Token> Parser::peek(int ahead) const
{
    if (m_index + ahead >= m_tokens.size())
    {
        return {};
    }
    
    return m_tokens.at(m_index + ahead);
}

Token Parser::consume()
{
    return m_tokens.at(m_index++);
}