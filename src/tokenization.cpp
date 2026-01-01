#include "tokenization.hpp"

#include <map>

#include "util.hpp"

Tokenizer::Tokenizer(const std::string& src)
    :m_src(src)
{ }

std::vector<Token> Tokenizer::tokenize()
{
    std::vector<Token> tokens {};

    const std::map<char, TokenType> nonAlphaTokenMap {
        {';', TokenType::semi},
        {'(', TokenType::paren_open},
        {')', TokenType::paren_close},
        {'=', TokenType::eq},
        {'+', TokenType::plus},
        {'*', TokenType::star},
        {'-', TokenType::dash},
        {'/', TokenType::slash_forward},
    };

    const std::map<std::string, TokenType> identifierTokenMap {
        {"print", TokenType::print},
        {"let", TokenType::let},
    };

    // Loop through all characters in string
    std::string buf {};
    while (peek().has_value())
    {
        // Check if identifier
        if (std::isalpha(peek().value()))
        {
            buf.push_back(consume());
            // Add full identifier to buffer
            while (peek().has_value() && std::isalnum(peek().value()))
            {
                buf.push_back(consume());
            }

            // Find correct identifier and add to token vector
            if (identifierTokenMap.contains(buf))
            {
                tokens.push_back({.type = identifierTokenMap.at(buf)});
            }
            else
            {
                tokens.push_back({.type = TokenType::ident, .value = buf});
            }

            // Reset buffer
            buf.clear();
        }
        // Check if num
        else if (std::isdigit(peek().value()))
        {
            buf.push_back(consume());
            while (peek().has_value() && (std::isdigit(peek().value()) || peek().value() == '.'))
            {
                buf.push_back(consume());
            }

            tokens.push_back({.type = TokenType::num_lit, .value = buf});
            buf.clear();
        }
        // Skip white space
        else if (std::isspace(peek().value()))
        {
            consume();
        }
        // Check for single character non-alpha tokens
        else if (nonAlphaTokenMap.contains(peek().value()))
        {
            tokens.push_back({.type = nonAlphaTokenMap.at(consume())});
        }
        // Invalid character
        else
        {
            compilation_error(std::string("Invalid character '") + peek().value() + '\'');
        }
    }

    m_index = 0;
    return tokens;
}

std::optional<int> Tokenizer::bin_prec(TokenType type)
{
    switch (type)
    {
    case TokenType::plus:
    case TokenType::dash:
        return 0;
    case TokenType::star:
    case TokenType::slash_forward:
        return 1;
    default:
        return {};
    }
}

std::optional<char> Tokenizer::peek(int ahead) const
{
    if (m_index + ahead >= m_src.length())
    {
        return {};
    }
    
    return m_src.at(m_index + ahead);
}

char Tokenizer::consume()
{
    return m_src.at(m_index++);
}

void Tokenizer::consume(int amount)
{
    m_index += amount;
}