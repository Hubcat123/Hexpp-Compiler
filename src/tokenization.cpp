#include "tokenization.hpp"

#include <map>

#include "util.hpp"

Tokenizer::Tokenizer(const std::string& src)
    :m_src(src)
{ }

std::vector<Token> Tokenizer::tokenize()
{
    std::vector<Token> tokens {};

    const std::map<char, TokenType> singleNonAlphaTokenMap {
        {';', TokenType::semi},
        {'(', TokenType::paren_open},
        {')', TokenType::paren_close},
        {'=', TokenType::eq},
        {'+', TokenType::plus},
        {'*', TokenType::star},
        {'-', TokenType::dash},
        {'/', TokenType::slash_forward},
        {'{', TokenType::curly_open},
        {'}', TokenType::curly_close},
        {'<', TokenType::angle_open},
        {'>', TokenType::angle_close},
        {',', TokenType::comma},
        {'.', TokenType::dot},
        {'!', TokenType::not_},
        {'%', TokenType::modulus},
    };

    const std::map<std::string, TokenType> doubleNonAlphaTokenMap {
        {"==", TokenType::double_eq},
        {"--", TokenType::double_dash},
        {"++", TokenType::double_plus},
        {"+=", TokenType::plus_eq},
        {"-=", TokenType::dash_eq},
        {"*=", TokenType::star_eq},
        {"/=", TokenType::fslash_eq},
        {"&&", TokenType::double_amp},
        {"||", TokenType::double_bar},
        {"!=", TokenType::not_eq_},
        {"<=", TokenType::oangle_eq},
        {">=", TokenType::cangle_eq},
        {"%=", TokenType::mod_eq},
    };

    const std::map<std::string, TokenType> identifierTokenMap {
        {"print", TokenType::print},
        {"let", TokenType::let},
        {"if", TokenType::if_},
        {"else", TokenType::else_},
        {"while", TokenType::while_},
        {"null", TokenType::null_lit},
        {"pow", TokenType::pow},
        {"vec", TokenType::vec},
        {"self", TokenType::self},
        {"pos", TokenType::pos},
        {"mine", TokenType::mine},
        {"forward", TokenType::forward},
        {"eye_pos", TokenType::eye_pos},
        {"block_raycast", TokenType::block_raycast},
        {"block_raycast_from", TokenType::block_raycast_from},
        {"block_normal_raycast_from", TokenType::block_normal_raycast_from},
        {"summon_light", TokenType::summon_light},
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
                tokens.push_back({.type = TokenType::bool_lit, .value = buf, .line = m_curr_line});
            }
            else if (identifierTokenMap.contains(buf))
            {
                tokens.push_back({.type = identifierTokenMap.at(buf), .line = m_curr_line});
            }
            else
            {
                tokens.push_back({.type = TokenType::ident, .value = buf, .line = m_curr_line});
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

            tokens.push_back({.type = TokenType::num_lit, .value = buf, .line = m_curr_line});
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

std::optional<int> Tokenizer::bin_prec(TokenType type)
{
    switch (type)
    {
    case TokenType::plus_eq:
    case TokenType::dash_eq:
    case TokenType::star_eq:
    case TokenType::fslash_eq:
    case TokenType::mod_eq:
    case TokenType::eq:
        return 0;
    case TokenType::double_bar:
        return 1;
    case TokenType::double_amp:
        return 2;
    case TokenType::double_eq:
    case TokenType::not_eq_:
        return 3;
    case TokenType::angle_open:
    case TokenType::angle_close:
    case TokenType::oangle_eq:
    case TokenType::cangle_eq:
        return 4;
    case TokenType::plus:
    case TokenType::dash:
        return 5;
    case TokenType::star:
    case TokenType::slash_forward:
    case TokenType::modulus:
        return 6;
    case TokenType::dot:
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