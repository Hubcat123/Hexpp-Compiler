#pragma once

#include "parser.hpp"

#include <sstream>
#include <stack>

class Generator {
public:
    Generator(const NodeProg* root);

    std::string generate();

    void gen_func(const NodeFunc* func);
    void gen_bin_expr(const NodeExprBin* expr_bin);
    void gen_term(const NodeTerm* term);
    void gen_expr(const NodeExpr* expr);
    void gen_stmt(const NodeStmt* stmt);
    void gen_prog();
    
    void try_gen_x_exprs(std::vector<NodeExpr*> exprs, int correct_amount);
    void pop(int amount = 1);
    void begin_scope();
    void end_scope();

    void additive_distilation();
    void alidades_purification();
    void archers_distilation();
    void architects_distilation();
    void augurs_purification();
    void break_block();
    void compass_purification();
    void compass_purification_II();
    void division_distilation();
    void fishermans_gambit();
    void fishermans_gambit_II();
    void flocks_reflection();
    void maximus_distilation();
    void minds_reflection();
    void minimus_distilation();
    void multiplicative_distilation();
    void numerical_reflection(std::string value);
    void power_distilation();
    void reveal();
    void subtractive_distilation();
    void vector_exaltation();
private:

    struct Var {
        std::string name;
        size_t stack_loc;
    };

    struct Scope {
        size_t stack_size;
        size_t var_num;
    };

    const NodeProg* m_prog;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::vector<Var> m_vars {};
    std::stack<Scope> m_scopes {};
};