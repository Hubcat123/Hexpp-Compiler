#pragma once

#include <string>
#include <vector>
#include <optional>

enum class TokenType_ {
    num_lit, paren_open, paren_close, semi, ident, let, eq, plus, star, dash, slash_forward, curly_open, curly_close, if_, angle_open, angle_close, comma, else_, while_, dot, double_eq,
    double_dash, double_plus, plus_eq, dash_eq, star_eq, fslash_eq, double_amp, double_bar, not_eq_, oangle_eq, cangle_eq, mod_eq, not_, modulus, null_lit, bool_lit, void_, ret, return_,
    vec, self, pos, mine, print, pow, eye_pos, forward, block_raycast, block_normal_raycast, summon_light, block_raycast_from, block_normal_raycast_from
};

struct Token {
    TokenType_ type;
    std::optional<std::string> value {};
    size_t line;
};

class Tokenizer
{
public:
    Tokenizer(const std::string& src);

    std::vector<Token> tokenize();

    static std::optional<int> bin_prec(TokenType_ type);
private:
    std::optional<char> peek(int ahead = 0) const;
    char consume();
    void consume(int amount);

    const std::string m_src;
    size_t m_index = 0;
    size_t m_curr_line = 1;
};