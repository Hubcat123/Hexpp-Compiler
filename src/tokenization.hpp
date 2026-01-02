#pragma once

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    num_lit, paren_open, paren_close, semi, ident, let, eq, plus, star, dash, slash_forward, curly_open, curly_close, if_, angle_open, angle_close, comma, else_,
    vec, self, pos, mine, print, pow, eye_pos, forward, block_raycast, block_normal_raycast
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
    size_t line;
};

class Tokenizer
{
public:
    Tokenizer(const std::string& src);

    std::vector<Token> tokenize();

    static std::optional<int> bin_prec(TokenType type);
private:
    std::optional<char> peek(int ahead = 0) const;
    char consume();
    void consume(int amount);

    const std::string m_src;
    size_t m_index = 0;
    size_t m_curr_line = 1;
};