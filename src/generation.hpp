#pragma once

#include "parser.hpp"

#include <sstream>
#include <stack>

class Generator {
public:
    Generator(const NodeProg* root);

    std::string generate();

    void gen_bin_expr(const NodeExprBin* expr_bin);
    void gen_term(const NodeTerm* term);
    void gen_expr(const NodeExpr* expr);
    void gen_stmt(const NodeStmt* stmt);
    void gen_prog();
    
    void pop(int amount = 1);
    void begin_scope();
    void end_scope();

    void additive_distilation();
    void division_distilation();
    void fishermans_gambit_II();
    void multiplicative_distilation();
    void numerical_reflection(std::string value);
    void reveal();
    void subtractive_distilation();
private:

    struct Var {
        std::string name;
        size_t stack_loc;
    };

    const NodeProg* m_prog;
    std::stringstream m_output;
    size_t m_stack_size;
    std::vector<Var> m_vars {};
    std::stack<size_t> m_scopes {};
};