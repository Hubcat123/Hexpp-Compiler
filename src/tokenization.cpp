#include "tokenization.hpp"

#include <map>

#include "util.hpp"

Tokenizer::Tokenizer(const std::string& src)
    :m_src(src)
{ }

std::vector<Token> Tokenizer::tokenize()
{
    std::vector<Token> tokens {};

    const std::map<char, TokenType_> singleNonAlphaTokenMap {
        {';', TokenType_::semi},
        {'(', TokenType_::paren_open},
        {')', TokenType_::paren_close},
        {'=', TokenType_::eq},
        {'+', TokenType_::plus},
        {'*', TokenType_::star},
        {'-', TokenType_::dash},
        {'/', TokenType_::slash_forward},
        {'{', TokenType_::curly_open},
        {'}', TokenType_::curly_close},
        {'<', TokenType_::angle_open},
        {'>', TokenType_::angle_close},
        {',', TokenType_::comma},
        {'.', TokenType_::dot},
        {'!', TokenType_::not_},
        {'%', TokenType_::modulus},
        {'[', TokenType_::square_open},
        {']', TokenType_::square_close},
    };

    const std::map<std::string, TokenType_> doubleNonAlphaTokenMap {
        {"==", TokenType_::double_eq},
        {"--", TokenType_::double_dash},
        {"++", TokenType_::double_plus},
        {"+=", TokenType_::plus_eq},
        {"-=", TokenType_::dash_eq},
        {"*=", TokenType_::star_eq},
        {"/=", TokenType_::fslash_eq},
        {"&&", TokenType_::double_amp},
        {"||", TokenType_::double_bar},
        {"!=", TokenType_::not_eq_},
        {"<=", TokenType_::oangle_eq},
        {">=", TokenType_::cangle_eq},
        {"%=", TokenType_::mod_eq},
    };

    const std::map<std::string, TokenType_> identifierTokenMap {
        {"let", TokenType_::let},
        {"if", TokenType_::if_},
        {"else", TokenType_::else_},
        {"while", TokenType_::while_},
        {"null", TokenType_::null_lit},
        {"void", TokenType_::void_},
        {"ret", TokenType_::ret},
        {"return", TokenType_::return_},
    };

    // Loop through all characters in string
    std::string buf {};
    while (peek().has_value())
    {
        // Check if single line comment
        if (peek().value() == '/' &&
            peek(1).has_value() && peek(1).value() == '/')
        {
            consume(2);
            while (peek().has_value() && peek().value() != '\n')
            {
                consume();
            }

            if (peek().has_value())
            {
                consume();
            }
        }
        // Check if multi line comment
        else if (
            peek().value() == '/' &&
            peek(1).has_value() && peek(1).value() == '*')
        {
            consume(2);

            do
            {
                while (peek().has_value() && peek().value() != '*')
                {
                    consume();
                }

                if (peek().has_value())
                {
                    consume();
                }
            }
            while (peek().has_value() && peek().value() != '/');

            if (peek().has_value())
            {
                consume();
            }
        }
        // Check if identifier
        else if (std::isalpha(peek().value()) || peek().value() == '_')
        {
            buf.push_back(consume());
            // Add full identifier to buffer
            while (peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_'))
            {
                buf.push_back(consume());
            }

            // Find correct identifier and add to token vector
            if (buf == "true" || buf == "false")
            {
                tokens.push_back({.type = TokenType_::bool_lit, .value = buf, .line = m_curr_line});
            }
            else if (identifierTokenMap.contains(buf))
            {
                tokens.push_back({.type = identifierTokenMap.at(buf), .line = m_curr_line});
            }
            else
            {
                tokens.push_back({.type = TokenType_::ident, .value = buf, .line = m_curr_line});
            }

            // Reset buffer
            buf.clear();
        }
        // Check if num
        else if (std::isdigit(peek().value()) || (peek(1).has_value() && peek().value() == '-' && std::isdigit(peek(1).value())))
        {
            buf.push_back(consume());
            bool foundDecimal = false;
            while (peek().has_value() && (std::isdigit(peek().value()) || (!foundDecimal && peek().value() == '.')))
            {
                if (peek().value() == '.')
                {
                    foundDecimal = true;
                }

                buf.push_back(consume());
            }

            tokens.push_back({.type = TokenType_::num_lit, .value = buf, .line = m_curr_line});
            buf.clear();
        }
        // Skip white space
        else if (std::isspace(peek().value()))
        {
            consume();
        }
        // Check for two character non-letter tokens
        else if (peek(1).has_value() && doubleNonAlphaTokenMap.contains(std::string(1, peek().value()) + peek(1).value()))
        {
            tokens.push_back({.type = doubleNonAlphaTokenMap.at(std::string(1, peek().value()) + peek(1).value()), .line = m_curr_line});
            consume(2);
        }
        // Check for single character non-letter tokens
        else if (singleNonAlphaTokenMap.contains(peek().value()))
        {
            tokens.push_back({.type = singleNonAlphaTokenMap.at(consume()), .line = m_curr_line});
        }
        // Invalid character
        else
        {
            compilation_error(std::string("Invalid character '") + peek().value() + '\'', m_curr_line);
        }
    }

    m_index = 0;
    return tokens;
}

std::optional<int> Tokenizer::bin_prec(TokenType_ type)
{
    switch (type)
    {
    case TokenType_::plus_eq:
    case TokenType_::dash_eq:
    case TokenType_::star_eq:
    case TokenType_::fslash_eq:
    case TokenType_::mod_eq:
    case TokenType_::eq:
        return 0;
    case TokenType_::double_bar:
        return 1;
    case TokenType_::double_amp:
        return 2;
    case TokenType_::double_eq:
    case TokenType_::not_eq_:
        return 3;
    case TokenType_::angle_open:
    case TokenType_::angle_close:
    case TokenType_::oangle_eq:
    case TokenType_::cangle_eq:
        return 4;
    case TokenType_::plus:
    case TokenType_::dash:
        return 5;
    case TokenType_::star:
    case TokenType_::slash_forward:
    case TokenType_::modulus:
        return 6;
    case TokenType_::dot:
        return 7;
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
    if (m_src.at(m_index) == '\n')
    {
        ++m_curr_line;
    }

    return m_src.at(m_index++);
}

void Tokenizer::consume(int amount)
{
    for (int i = 0; i < amount; ++i)
    {
        consume();
    }
}