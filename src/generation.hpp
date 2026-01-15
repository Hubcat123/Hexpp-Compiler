#pragma once

#include "parser.hpp"

#include <sstream>
#include <stack>

class Generator {
public:
    Generator(const NodeProg* root);

    std::string generate();

    void gen_func(const NodeFunc* func);
    // Returns whether func is void func
    bool gen_defined_func(const NodeDefinedFunc* func);
    void gen_bin_expr(const NodeExprBin* expr_bin);
    void gen_term(const NodeTerm* term);
    void gen_expr(const NodeExpr* expr);
    void gen_stmt(const NodeStmt* stmt);
    void gen_func_def(const NodeFunctionDef* func_def);
    void gen_prog();
    
    void try_gen_x_exprs(std::vector<NodeExpr*> exprs, int correct_amount, size_t line);
    void pop(int amount = 1);
    void begin_scope();
    void end_scope();
    void end_scopes_return(bool has_ret_value);
    void dec_func(bool is_void, std::string name, int num_params, size_t line);

    void additive_distilation();
    void alidades_purification();
    void archers_distilation();
    void architects_distilation();
    void augurs_purification();
    void break_block();
    void compass_purification();
    void compass_purification_II();
    // AND
    void conjunction_distilation();
    void conjure_light();
    // OR
    void disjunction_distilation();
    void division_distilation();
    void equality_distilation();
    void false_reflection();
    void fishermans_gambit();
    void fishermans_gambit_II();
    void flocks_reflection();
    void gemini_decomposition();
    // Store in Raven's Mind
    void huginns_gambit();
    void inequality_distilation();
    void jesters_gambit();
    void maximus_distilation();
    void maximus_distilation_II();
    void minds_reflection();
    void minimus_distilation();
    void minimus_distilation_II();
    void modulus_distilation();
    void multiplicative_distilation();
    // Retrieve from Raven's Mind
    void muninns_reflection();
    void negation_purification();
    void nullary_reflection();
    void numerical_reflection(std::string value);
    void power_distilation();
    void reveal();
    void selection_distilation();
    void subtractive_distilation();
    void surgeons_exaltation();
    void true_reflection();
    void vacant_reflection();
    void vector_exaltation();

    bool generating_void_function = false;
private:

    struct Var {
        std::string name;
        size_t stack_loc;
    };

    struct Func {
        bool is_void;
        std::string name;
        int num_params;
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
    std::vector<Var> m_global_vars {};
    std::vector<Func> m_funcs {};
    std::vector<Scope> m_scopes {};

    size_t m_function_start_scope;
    size_t m_function_num_params;
};