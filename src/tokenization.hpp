#pragma once

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    print, num_lit, paren_open, paren_close, semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer
{
public:
    Tokenizer(const std::string& src);

    std::vector<Token> tokenize();
private:
    std::optional<char> peek(int ahead = 0) const;
    char consume();

    const std::string m_src;
    size_t m_index = 0;
};