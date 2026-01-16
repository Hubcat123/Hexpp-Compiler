#include "generation.hpp"

#include <sstream>

#include "util.hpp"

Generator::Generator(const NodeProg* prog)
    :m_prog(prog)
{ }

std::string Generator::generate()
{
    gen_prog();

    return m_output.str();
}

void Generator::gen_func(const NodeFunc* func)
{
    switch (func->func_type)
    {
    case TokenType_::print:
        try_gen_x_exprs(func->exprs, 1, func->line);
        reveal();
        pop();
        break;
    case TokenType_::pow:
        try_gen_x_exprs(func->exprs, 2, func->line);
        power_distilation();
        break;
    case TokenType_::vec:
        try_gen_x_exprs(func->exprs, 3, func->line);
        vector_exaltation();
        break;
    case TokenType_::self:
        try_gen_x_exprs(func->exprs, 0, func->line);
        minds_reflection();
        break;
    case TokenType_::pos:
        try_gen_x_exprs(func->exprs, 0, func->line);
        compass_purification_II();
        break;
    case TokenType_::mine:
        try_gen_x_exprs(func->exprs, 1, func->line);
        break_block();
        break;
    case TokenType_::forward:
        try_gen_x_exprs(func->exprs, 0, func->line);
        alidades_purification();
        break;
    case TokenType_::eye_pos:
        try_gen_x_exprs(func->exprs, 0, func->line);
        compass_purification();
        break;
    case TokenType_::block_raycast:
        try_gen_x_exprs(func->exprs, 2, func->line);
        archers_distilation();
        break;
    case TokenType_::block_raycast_from:
        try_gen_x_exprs(func->exprs, 1, func->line);
        gemini_decomposition();
        compass_purification();
        jesters_gambit();
        alidades_purification();
        archers_distilation();
        break;
    case TokenType_::block_normal_raycast:
        try_gen_x_exprs(func->exprs, 2, func->line);
        architects_distilation();
        break;
    case TokenType_::block_normal_raycast_from:
        try_gen_x_exprs(func->exprs, 1, func->line);
        gemini_decomposition();
        compass_purification();
        jesters_gambit();
        alidades_purification();
        architects_distilation();
        break;
    case TokenType_::summon_light:
        try_gen_x_exprs(func->exprs, 1, func->line);
        conjure_light();
        break;
    }
}

bool Generator::gen_defined_func(const NodeDefinedFunc* func)
{
    // Find function being called
    std::vector<Func>::iterator iter = std::find_if(m_funcs.begin(), m_funcs.end(), [&](const Func& _func){
        return _func.name == func->ident.value.value() && _func.num_params == func->exprs.size();});
    if (iter == m_funcs.end())
    {
        compilation_error(std::string("No function defined with this name with the passed number of parameters: ") + func->ident.value.value(), func->line);
    }

    // Generate expressions
    for (NodeExpr* expr : func->exprs)
    {
        gen_expr(expr);
    }

    // Execute function code
    muninns_reflection();
    numerical_reflection(std::to_string(iter->stack_loc));
    selection_distilation();
    // Iris' Gambit
    add_pattern("Iris' Gambit", 0);

    // Account for function cleaning up exprs
    m_stack_size -= func->exprs.size();

    // Account for expression left on stack from non-void function and function iota being consumed
    if (iter->is_void)
    {
        --m_stack_size;
    }

    return iter->is_void;
}

void Generator::gen_bin_expr(const NodeExprBin* expr_bin)
{
    // If binary expression is a type of assignment
    if (expr_bin->op_type == TokenType_::eq || expr_bin->op_type == TokenType_::plus_eq || expr_bin->op_type == TokenType_::dash_eq || expr_bin->op_type == TokenType_::star_eq
         || expr_bin->op_type == TokenType_::fslash_eq || expr_bin->op_type == TokenType_::mod_eq)
    {
        if (!std::holds_alternative<NodeTerm*>(expr_bin->lhs->var))
        {
            compilation_error("Expected identifier", expr_bin->line);
        }

        NodeTerm* term = std::get<NodeTerm*>(expr_bin->lhs->var);
        if (!std::holds_alternative<NodeTermIdent*>(term->var))
        {
            compilation_error("Expected identifier", expr_bin->line);
        }

        Token ident = std::get<NodeTermIdent*>(term->var)->ident;

        gen_assignment(std::get<NodeTermIdent*>(term->var)->ident, expr_bin->rhs, expr_bin->op_type, expr_bin->line);

        return;
    }

    gen_expr(expr_bin->lhs);

    // If binary expression is calling a member function
    if (expr_bin->op_type == TokenType_::dot)
    {
        // If rhs is a term
        if (std::holds_alternative<NodeTerm*>(expr_bin->rhs->var))
        {
            NodeTerm* term = std::get<NodeTerm*>(expr_bin->rhs->var);
            // If term is a function
            if (std::holds_alternative<NodeTermInbuiltFunc*>(term->var))
            {
                NodeTermInbuiltFunc* term_func = std::get<NodeTermInbuiltFunc*>(term->var);

                if (!term_func->isMemberFunc)
                {
                    compilation_error("Expected member function", expr_bin->line);
                }

                gen_func(term_func->func);
                return;
            }
            else
            {
                compilation_error("Expected member function", expr_bin->line);
            }
        }
        else
        {
            compilation_error("Expected member function", expr_bin->line);
        }
    }

    gen_expr(expr_bin->rhs);

    switch(expr_bin->op_type)
    {
    case TokenType_::double_eq:
        equality_distilation();
        break;
    case TokenType_::not_eq_:
        inequality_distilation();
        break;
    case TokenType_::angle_open:
        minimus_distilation();
        break;
    case TokenType_::oangle_eq:
        minimus_distilation_II();
        break;
    case TokenType_::angle_close:
        maximus_distilation();
        break;
    case TokenType_::cangle_eq:
        maximus_distilation_II();
        break;
    case TokenType_::plus:
        additive_distilation();
        break;
    case TokenType_::dash:
        subtractive_distilation(); 
        break;
    case TokenType_::star:
        multiplicative_distilation();
        break;
    case TokenType_::slash_forward:
        division_distilation();
        break;
    case TokenType_::modulus:
        modulus_distilation();
        break;
    case TokenType_::double_amp:
        conjunction_distilation();
        break;
    case TokenType_::double_bar:
        disjunction_distilation();
        break;
    }
}

void Generator::gen_term(const NodeTerm* term)
{
    struct TermVisitor {
        Generator& gen;
        TermVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeTermUn* term_un)
        {
            // If unary expression is a type of assignment
            if (term_un->op_type == TokenType_::double_plus || term_un->op_type == TokenType_::double_dash)
            {
                if (!term_un->ident.has_value())
                {
                    compilation_error("Expected identifier", term_un->line);
                }

                gen.gen_assignment(term_un->ident.value(), (float)((term_un->op_type == TokenType_::double_plus) ? 1 : -1), TokenType_::plus_eq, term_un->line);

                return;
            }

            gen.gen_term(term_un->term);

            switch (term_un->op_type)
            {
            case TokenType_::dash:
                gen.numerical_reflection("-1");
                gen.multiplicative_distilation();
                break;
            case TokenType_::not_:
                gen.negation_purification();
                break;
            }
        }

        void operator()(const NodeTermUnPost* term_un_post)
        {
            gen.gen_assignment(term_un_post->ident, (float)((term_un_post->op_type == TokenType_::double_plus) ? 1 : -1), TokenType_::plus_eq, term_un_post->line, true);
        }

        void operator()(const NodeTermNumLit* term_int_lit)
        {
            gen.numerical_reflection(term_int_lit->num_lit.value.value());
        }

        void operator()(const NodeTermBoolLit* term_bool_lit)
        {
            if (term_bool_lit->bool_.value == "true")
            {
                gen.true_reflection();
            }
            else
            {
                gen.false_reflection();
            }
        }

        void operator()(const NodeTermNullLit* term_null_lit)
        {
            gen.nullary_reflection();
        }

        void operator()(const NodeTermIdent* term_ident)
        {
            bool is_global = false;
            std::vector<Var>::iterator iter = std::find_if(gen.m_vars.begin(), gen.m_vars.end(),
                [&](const Var& var){ return var.name == term_ident->ident.value.value(); });
            
            if (iter == gen.m_vars.end())
            {
                is_global = true;
                iter = std::find_if(gen.m_global_vars.begin(), gen.m_global_vars.end(),
                    [&](const Var& var){ return var.name == term_ident->ident.value.value(); });
                
                if (iter == gen.m_global_vars.end())
                {
                    compilation_error(std::string("Undeclared identifier: ") + term_ident->ident.value.value(), term_ident->line);
                }
            }

            Var& var = *iter;

            if (is_global)
            {
                gen.muninns_reflection();
                gen.numerical_reflection(std::to_string(var.stack_loc));
                gen.selection_distilation();
            }
            else
            {
                gen.numerical_reflection(std::to_string(gen.m_stack_size - 1 - var.stack_loc));
                gen.fishermans_gambit_II();
            }
        }

        void operator()(const NodeTermParen* term_paren)
        {
            gen.gen_expr(term_paren->expr);
        }

        void operator()(const NodeTermInbuiltFunc* term_func)
        {
            if (term_func->isMemberFunc)
            {
                compilation_error("Must call member functions on a member", term_func->line);
            }

            gen.gen_func(term_func->func);
        }

        void operator()(const NodeTermCallFunc* call_func)
        {
            if (gen.gen_defined_func(call_func->func))
            {
                compilation_error("Calling void function as return function", call_func->line);
            }
        }
    };

    TermVisitor visitor(*this);
    std::visit(visitor, term->var);
}

void Generator::gen_expr(const NodeExpr* expr)
{
    struct ExprVisitor {
        Generator& gen;
        ExprVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeTerm* term)
        {
            gen.gen_term(term);
        }

        void operator()(const NodeExprBin* expr_bin)
        {
            gen.gen_bin_expr(expr_bin);
        }
    };

    ExprVisitor visitor(*this);
    std::visit(visitor, expr->var);
}

void Generator::gen_stmt(const NodeStmt* stmt)
{
    struct StmtVisitor {
        Generator& gen;
        StmtVisitor(Generator& _gen) :gen(_gen) {}

        void operator()(const NodeStmtInbuiltFunc* stmt_func)
        {
            gen.gen_func(stmt_func->func);
        }

        void operator()(const NodeStmtCallFunction* call_func)
        {
            if (!gen.gen_defined_func(call_func->func))
            {
                gen.pop();
            }
        }

        void operator()(const NodeStmtReturn* stmt_ret)
        {
            // Error check for passing/not passing expression into return
            if (gen.generating_void_function && stmt_ret->expr.has_value())
            {
                compilation_error("Returning expression from void function", stmt_ret->line);
            }

            if (!gen.generating_void_function && !stmt_ret->expr.has_value())
            {
                compilation_error("Return must have expression in non-void functions", stmt_ret->line);
            }

            // Generate expression if there is one
            if (stmt_ret->expr.has_value())
            {
                gen.gen_expr(stmt_ret->expr.value());
            }

            // Remove scope items from stack
            gen.end_scopes_return(stmt_ret->expr.has_value());

            // If there's an expression on the stack, swap it with jump iota
            if (stmt_ret->expr.has_value())
            {
                gen.jesters_gambit();
            }

            // Execute jump iota
            gen.add_pattern("Hermes' Gambit", 0);
        }

        void operator()(const NodeExpr* stmt_expr)
        {
            gen.gen_expr(stmt_expr);
            gen.pop();
        }

        void operator()(const NodeStmtLet* stmt_let)
        {
            if (std::find_if(gen.m_vars.cbegin(), gen.m_vars.cend(), [&](const Var& var){return var.name == stmt_let->ident.value.value();}) != gen.m_vars.cend())
            {
                compilation_error(std::string("Identifier already used: ") + stmt_let->ident.value.value(), stmt_let->line);
            }

            gen.gen_expr(stmt_let->expr);
            gen.m_vars.push_back(Var{.name = stmt_let->ident.value.value(), .stack_loc = gen.m_stack_size - 1});
        }

        void operator()(const NodeStmtIf* stmt_if)
        {
            // Evaluate expression
            gen.gen_expr(stmt_if->expr);
            gen.augurs_purification();
            --gen.m_stack_size;

            // Generate statement
            gen.begin_scope();
            gen.add_pattern("{", 0);
            gen.increase_indent();
            gen.gen_stmt(stmt_if->stmt);
            gen.decrease_indent();
            gen.add_pattern("}", 0);
            gen.end_scope();

            // Potentially generate else statement
            if (stmt_if->else_stmt == nullptr)
            {
                gen.add_pattern("Vacant Reflection", 0);
            }
            else
            {
                gen.add_pattern("{", 0);
                gen.increase_indent();
                gen.begin_scope();
                gen.gen_stmt(stmt_if->else_stmt);
                gen.end_scope(); 
                gen.decrease_indent();
                gen.add_pattern("}", 0);
            }
            
            // Perform bool comparison and execute
            gen.add_pattern("Augur's Exaltation", 0);
            gen.add_pattern("Hermes' Gambit", 0);
        }

        void operator()(const NodeStmtWhile* stmt_while)
        {
            gen.add_pattern("{", 0);
            gen.increase_indent();
            ++gen.m_stack_size;
            gen.gen_expr(stmt_while->expr);

            // Exit loop if false
            gen.augurs_purification();
            gen.add_pattern("Vacant Reflection", 0);
            gen.add_pattern("{", 0);
            gen.increase_indent();
            gen.add_pattern("Charon's Gambit", 0);
            gen.decrease_indent();
            gen.add_pattern("}", 0);
            gen.add_pattern("Flock's Disintegration", 0);
            gen.add_pattern("Augur's Exaltation", 0);
            gen.add_pattern("Hermes' Gambit", 0);
            
            // Generate statement
            --gen.m_stack_size;
            gen.begin_scope();
            gen.gen_stmt(stmt_while->stmt);
            gen.end_scope();

            // Execute next loop
            gen.add_pattern("Gemini Decomposition", 0);
            gen.add_pattern("Hermes' Gambit", 0);
            gen.decrease_indent();
            gen.add_pattern("}", 0);

            gen.add_pattern("Gemini Decomposition", 0);
            gen.add_pattern("Hermes' Gambit", 0);
            gen.pop();
        }

        void operator()(const NodeScope* stmt_scope)
        {
            gen.begin_scope();

            for (NodeStmt* stmt : stmt_scope->stmts)
            {
                gen.gen_stmt(stmt);
            }

            gen.end_scope();
        }
    };

    StmtVisitor visitor(*this);
    std::visit(visitor, stmt->var);
}

void Generator::gen_func_def(const NodeFunctionDef* func_def)
{
    // Visitor to extract function info
    struct FuncDefVisitor {
        Generator& gen;
        bool& is_void;
        std::vector<Token>& params;
        NodeScope*& scope;
        FuncDefVisitor (Generator& _gen, bool& _is_void, std::vector<Token>& _params, NodeScope*& _scope) :gen(_gen), is_void(_is_void), params(_params), scope(_scope) {}
        
        void operator()(const NodeFunctionDefVoid* func_void)
        {
            is_void = true;
            params = func_void->params;
            scope = func_void->scope;
        }

        void operator()(const NodeFunctionDefRet* func_ret)
        {
            is_void = false;
            params = func_ret->params;
            scope = func_ret->scope;
        }
    };

    // Extract function info
    bool is_void;
    std::vector<Token> params;
    NodeScope* scope;
    FuncDefVisitor visitor(*this, is_void, params, scope);
    std::visit(visitor, func_def->var);

    generating_void_function = is_void;
    m_function_start_scope = m_scopes.size();
    m_function_num_params = params.size();

    add_pattern("{", 0);
    increase_indent();
    begin_scope();

    // Treat top of the stack as params
    for (Token param : params)
    {
        m_vars.push_back(Var{.name = param.value.value(), .stack_loc = m_stack_size});
        ++m_stack_size;
    }

    // Account for jump iota from iris' gambit
    ++m_stack_size;

    // Generate stmts in function
    for (NodeStmt* stmt : scope->stmts)
    {
        gen_stmt(stmt);
    }

    // If function isn't void, then provide null return value by default
    if (is_void)
    {
        end_scope();
    }
    else
    {
        // Remove excess from stack
        end_scopes_return(false);
        // Destroy jump iota
        pop();
        // Add null to stack
        nullary_reflection();
    }
    decrease_indent();
    add_pattern("}", 1);
}

void Generator::gen_prog()
{
    // Gen global var exprs
    for (NodeGlobalLet* global_let : m_prog->vars)
    {
        if (std::find_if(m_global_vars.cbegin(), m_global_vars.cend(), [&](const Var& var){return var.name == global_let->ident.value.value();}) != m_global_vars.cend())
        {
            compilation_error(std::string("Global identifier already used: ") + global_let->ident.value.value(), global_let->line);
        }

        gen_expr(global_let->expr);

        // Register temporarily as local var so they can reference other global vars during declaration
        m_vars.push_back(Var{.name = global_let->ident.value.value(), .stack_loc = m_vars.size()});
    }

    // Clear temp local vars
    m_vars.clear();

    // Mark global variables as declared
    for (NodeGlobalLet* global_let : m_prog->vars)
    {
        m_global_vars.push_back(Var{.name = global_let->ident.value.value(), .stack_loc = m_global_vars.size()});
    }

    // Account for main's jump iota
    ++m_stack_size;

    // Visitor for declaring functions
    struct FuncDefVisitor {
        Generator& gen;
        FuncDefVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeFunctionDefVoid* func_void)
        {
            gen.dec_func(true, func_void->ident.value.value(), func_void->params.size(), func_void->line);
        }

        void operator()(const NodeFunctionDefRet* func_ret)
        {
            gen.dec_func(false, func_ret->ident.value.value(), func_ret->params.size(), func_ret->line);
        }
    };
    FuncDefVisitor visitor(*this);

    // Gen function declarations
    for (NodeFunctionDef* func_def : m_prog->funcs)
    {
        std::visit(visitor, func_def->var);
    }

    // Gen functions
    for (NodeFunctionDef* func_def : m_prog->funcs)
    {
        gen_func_def(func_def);
    }

    // Store functions and global vars in list for raven's mind
    if (m_global_vars.size() + m_funcs.size() > 0)
    {
        numerical_reflection(std::to_string(m_global_vars.size() + m_funcs.size()));
        add_pattern("Flock's Gambit", -m_global_vars.size() - m_funcs.size());
    }
    else
    {
        vacant_reflection();
    }
    huginns_gambit();

    // Remove accounting of jump iota so it isn't double counted
    --m_stack_size;

    // Gen main
    gen_func_def(m_prog->main_);

    // Execute main
    add_pattern("Iris' Gambit", -1);
}



void Generator::try_gen_x_exprs(std::vector<NodeExpr*> exprs, int correct_amount, size_t line)
{
    if (exprs.size() != correct_amount)
    {
        compilation_error("Incorrect number of arguments passed into function", line);
    }

    for (NodeExpr* expr : exprs)
    {
        gen_expr(expr);
    }
}

void Generator::gen_assignment(const Token ident, const std::variant<const NodeExpr*, const float> value, TokenType_ op, size_t line, bool is_post)
{
    bool is_global = false;
    // Try to find local var
    std::vector<Var>::iterator iter = std::find_if(m_vars.begin(), m_vars.end(),
        [&](const Var& var){ return var.name == ident.value.value(); });
    
    if (iter == m_vars.end())
    {
        is_global = true;
        // Try to find global var
        iter = std::find_if(m_global_vars.begin(), m_global_vars.end(), 
            [&](const Var& var){ return var.name == ident.value.value(); });
        
        if (iter == m_global_vars.end())
        {
            compilation_error(std::string("Undeclared identifier: ") + ident.value.value(), line);
        }
    }

    // Add value to top of stack
    struct ValueVisitor {
        Generator& gen;
        ValueVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeExpr* expr)
        {
            gen.gen_expr(expr);
        }

        void operator()(const float num)
        {
            // Cut off decimal part if it's not used
            if (num == (int)num)
            {
                gen.numerical_reflection(std::to_string((int)num));
            }
            else
            {
                gen.numerical_reflection(std::to_string(num));
            }
        }
    };

    ValueVisitor visitor(*this);
    std::visit(visitor, value);

    Var& var = *iter;

    // If local
    int varDepth = m_stack_size - 1 - var.stack_loc;
    if (!is_global)
    {
        numerical_reflection(std::to_string(varDepth));
        fishermans_gambit();
    }
    // If global and not eq
    else if (op != TokenType_::eq)
    {
        muninns_reflection();
        numerical_reflection(std::to_string(var.stack_loc));
        selection_distilation();
    }

    // Make duplicate pre-op if it's a post-op
    if (is_post)
    {
        gemini_decomposition();
    }

    switch (op)
    {
    case TokenType_::eq:
        // Remove prev value from stack if local. Global var will be overwritten later
        if (!is_global)
        {
            pop();
        }
        break;
    case TokenType_::plus_eq:
        additive_distilation();
        break;
    case TokenType_::dash_eq:
        jesters_gambit();
        subtractive_distilation();
        break;
    case TokenType_::star_eq:
        multiplicative_distilation();
        break;
    case TokenType_::fslash_eq:
        jesters_gambit();
        division_distilation();
        break;
    case TokenType_::mod_eq:
        jesters_gambit();
        modulus_distilation();
        break;
    }
    
    // If local
    if (!is_global)
    {
        numerical_reflection(std::to_string(-varDepth + 1));

        // Don't duplicate down if post-op
        if (is_post)
        {
            fishermans_gambit();
        }
        else
        {
            fishermans_gambit_II();
        }
    }
    // If global
    else
    {
        // Don't duplicate if post-op
        if (!is_post)
        {
            gemini_decomposition();
        }

        muninns_reflection();
        numerical_reflection(std::to_string(var.stack_loc));
        rotation_gambit();
        surgeons_exaltation();
        huginns_gambit();
    }
}

void Generator::pop(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    add_pattern(std::string("Bookkeeper's Gambit: ") + std::string(amount, 'v'), -amount);
}

void Generator::begin_scope()
{
    m_scopes.push_back(Scope{.stack_size = m_stack_size, .var_num = m_vars.size()});
}

void Generator::end_scope()
{
    size_t pop_count = m_stack_size - m_scopes.back().stack_size;
    pop(pop_count);

    m_vars.resize(m_scopes.back().var_num);

    m_scopes.pop_back();
}

void Generator::end_scopes_return(bool has_ret_value)
{
    size_t pop_count = m_stack_size - m_scopes[m_function_start_scope].stack_size - (has_ret_value ? 1 : 0) - 1;

    // Bookkeepr's Gambit all stack elements except possible ret value and jump iota
    add_pattern(std::string("Bookkeeper's Gambit: ") + std::string(m_function_num_params, 'v') + '-' + std::string(pop_count - m_function_num_params, 'v') + (has_ret_value ? "-" : ""), 0);
}

void Generator::dec_func(bool is_void, std::string name, int num_params, size_t line)
{
    if (std::find_if(m_funcs.cbegin(), m_funcs.cend(), [&](const Func& func){return func.name == name && func.num_params == num_params;}) != m_funcs.cend())
    {
        compilation_error(std::string("Function with this name and number of parameters already declared: ") + name, line);
    }

    m_funcs.push_back(Func{.is_void = is_void, .name = name, .num_params = num_params, .stack_loc = m_global_vars.size() + m_funcs.size()});
}

void Generator::increase_indent()
{
    ++m_indent_level;
}

void Generator::decrease_indent()
{
    --m_indent_level;
}



void Generator::additive_distilation()
{
    add_pattern("Additive Distillation", -1);
}

void Generator::alidades_purification()
{
    add_pattern("Alidade's Purification", 0);
}

void Generator::archers_distilation()
{
    add_pattern("Archer's Distillation", -1);
}

void Generator::architects_distilation()
{
    add_pattern("Architect's Distillation", -1);
}

void Generator::augurs_purification()
{
    add_pattern("Augur's Purification", 0);
}

void Generator::break_block()
{
    add_pattern("Break Block", -1);
}

void Generator::compass_purification()
{
    add_pattern("Compass' Purification", 0);
}

void Generator::compass_purification_II()
{
    add_pattern("Compass' Purification II", 0);
}

void Generator::conjunction_distilation()
{
    add_pattern("Conjunction Distillation", -1);
}

void Generator::conjure_light()
{
    add_pattern("Conjure Light", -1);
}

void Generator::disjunction_distilation()
{
    add_pattern("Disjunction Distillation", -1);
}

void Generator::division_distilation()
{
    add_pattern("Division Distillation", -1);
}

void Generator::equality_distilation()
{
    add_pattern("Equality Distillation", -1);
}

void Generator::false_reflection()
{
    add_pattern("False Reflection", 1);
}

void Generator::fishermans_gambit()
{
    add_pattern("Fisherman's Gambit", -1);
}

void Generator::fishermans_gambit_II()
{
    add_pattern("Fisherman's Gambit II", 0);
}

void Generator::flocks_reflection()
{
    add_pattern("Flock's Reflection", 1);
}

void Generator::gemini_decomposition()
{
    add_pattern("Gemini Decomposition", 1);
}

void Generator::huginns_gambit()
{
    add_pattern("Huginn's Gambit", -1);
}

void Generator::inequality_distilation()
{
    add_pattern("Inequality Distillation", -1);
}

void Generator::jesters_gambit()
{
    add_pattern("Jester's Gambit", 0);
}

void Generator::maximus_distilation()
{
    add_pattern("Maximus Distillation", -1);
}

void Generator::maximus_distilation_II()
{
    add_pattern("Maximus Distillation II", -1);
}

void Generator::minds_reflection()
{
    add_pattern("Mind's Reflection", 1);
}

void Generator::minimus_distilation()
{
    add_pattern("Minimus Distillation", -1);
}

void Generator::minimus_distilation_II()
{
    add_pattern("Minimus Distillation II", -1);
}

void Generator::modulus_distilation()
{
    add_pattern("Modulus Distillation", -1);
}

void Generator::multiplicative_distilation()
{
    add_pattern("Multiplicative Distillation", -1);
}

void Generator::muninns_reflection()
{
    add_pattern("Muninn's Reflection", 1);
}

void Generator::negation_purification()
{
    add_pattern("Negation Purification", 0);
}

void Generator::nullary_reflection()
{
    add_pattern("Nullary Reflection", 1);
}

void Generator::numerical_reflection(std::string value)
{
    if (value.find('.') != std::string::npos)
    {
        has_non_integer_num = true;
    }

    add_pattern(std::string("Numerical Reflection: ") + value, 1);
}

void Generator::power_distilation()
{
    add_pattern("Power Distillation", -1);
}

void Generator::reveal()
{
    add_pattern("Reveal", 0);
}

void Generator::rotation_gambit()
{
    add_pattern("Rotation Gambit", 0);
}

void Generator::rotation_gambit_II()
{
    add_pattern("Rotation Gambit II", 0);
}

void Generator::selection_distilation()
{
    add_pattern("Selection Distillation", -1);
}

void Generator::subtractive_distilation()
{
    add_pattern("Subtractive Distillation", -1);
}

void Generator::surgeons_exaltation()
{
    add_pattern("Surgeon's Exaltation", -2);
}

void Generator::true_reflection()
{
    add_pattern("True Reflection", 1);
}

void Generator::vacant_reflection()
{
    add_pattern("Vacant Reflection", 1);
}

void Generator::vector_exaltation()
{
    add_pattern("Vector Exaltation", -2);
}

void Generator::add_pattern(std::string pattern, size_t stack_size_net)
{
    m_output << std::string(m_indent_level, '\t') << pattern << '\n';
    m_stack_size += stack_size_net;
}