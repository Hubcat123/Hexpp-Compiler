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
    case TokenType::print:
        try_gen_x_exprs(func->exprs, 1, func->line);
        reveal();
        pop();
        break;
    case TokenType::pow:
        try_gen_x_exprs(func->exprs, 2, func->line);
        power_distilation();
        break;
    case TokenType::vec:
        try_gen_x_exprs(func->exprs, 3, func->line);
        vector_exaltation();
        break;
    case TokenType::self:
        try_gen_x_exprs(func->exprs, 0, func->line);
        minds_reflection();
        break;
    case TokenType::pos:
        try_gen_x_exprs(func->exprs, 0, func->line);
        compass_purification_II();
        break;
    case TokenType::mine:
        try_gen_x_exprs(func->exprs, 1, func->line);
        break_block();
        break;
    case TokenType::forward:
        try_gen_x_exprs(func->exprs, 0, func->line);
        alidades_purification();
        break;
    case TokenType::eye_pos:
        try_gen_x_exprs(func->exprs, 0, func->line);
        compass_purification();
        break;
    case TokenType::block_raycast:
        try_gen_x_exprs(func->exprs, 2, func->line);
        archers_distilation();
        break;
    case TokenType::block_raycast_from:
        try_gen_x_exprs(func->exprs, 1, func->line);
        gemini_decomposition();
        compass_purification();
        jesters_gambit();
        alidades_purification();
        archers_distilation();
        break;
    case TokenType::block_normal_raycast:
        try_gen_x_exprs(func->exprs, 2, func->line);
        architects_distilation();
        break;
    case TokenType::block_normal_raycast_from:
        try_gen_x_exprs(func->exprs, 1, func->line);
        gemini_decomposition();
        compass_purification();
        jesters_gambit();
        alidades_purification();
        architects_distilation();
        break;
    case TokenType::summon_light:
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
    m_output << "Pattern \"qwaqde\"\n";

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
    if (expr_bin->op_type == TokenType::eq || expr_bin->op_type == TokenType::plus_eq || expr_bin->op_type == TokenType::dash_eq || expr_bin->op_type == TokenType::star_eq
         || expr_bin->op_type == TokenType::fslash_eq || expr_bin->op_type == TokenType::mod_eq)
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

        const std::vector<Var>::iterator iter = std::find_if(m_vars.begin(), m_vars.end(),
            [&](const Var& var){ return var.name == ident.value.value(); });
        
        if (iter == m_vars.end())
        {
            compilation_error(std::string("Undeclared identifier: ") + ident.value.value(), expr_bin->line);
        }

        gen_expr(expr_bin->rhs);
        Var& var = *iter;
        int varDepth = m_stack_size - 1 - var.stack_loc;
        numerical_reflection(std::to_string(varDepth));
        fishermans_gambit();

        switch (expr_bin->op_type)
        {
        case TokenType::eq:
            pop();
            break;
        case TokenType::plus_eq:
            additive_distilation();
            break;
        case TokenType::dash_eq:
            jesters_gambit();
            subtractive_distilation();
            break;
        case TokenType::star_eq:
            multiplicative_distilation();
            break;
        case TokenType::fslash_eq:
            jesters_gambit();
            division_distilation();
            break;
        case TokenType::mod_eq:
            jesters_gambit();
            modulus_distilation();
            break;
        }
        
        numerical_reflection(std::to_string(-varDepth + 1));
        fishermans_gambit_II();
        return;
    }

    gen_expr(expr_bin->lhs);

    // If binary expression is calling a member function
    if (expr_bin->op_type == TokenType::dot)
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
    case TokenType::double_eq:
        equality_distilation();
        break;
    case TokenType::not_eq_:
        inequality_distilation();
        break;
    case TokenType::angle_open:
        minimus_distilation();
        break;
    case TokenType::oangle_eq:
        minimus_distilation_II();
        break;
    case TokenType::angle_close:
        maximus_distilation();
        break;
    case TokenType::cangle_eq:
        maximus_distilation_II();
        break;
    case TokenType::plus:
        additive_distilation();
        break;
    case TokenType::dash:
        subtractive_distilation(); 
        break;
    case TokenType::star:
        multiplicative_distilation();
        break;
    case TokenType::slash_forward:
        division_distilation();
        break;
    case TokenType::modulus:
        modulus_distilation();
        break;
    case TokenType::double_amp:
        conjunction_distilation();
        break;
    case TokenType::double_bar:
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
            if (term_un->op_type == TokenType::double_plus || term_un->op_type == TokenType::double_dash)
            {
                if (!term_un->ident.has_value())
                {
                    compilation_error("Expected identifier", term_un->line);
                }

                const std::vector<Var>::iterator iter = std::find_if(gen.m_vars.begin(), gen.m_vars.end(),
                    [&](const Var& var){ return var.name == term_un->ident.value().value.value(); });
                
                if (iter == gen.m_vars.end())
                {
                    compilation_error(std::string("Undeclared identifier: ") + term_un->ident.value().value.value(), term_un->line);
                }

                Var& var = *iter;
                int varDepth = gen.m_stack_size - 1 - var.stack_loc;
                gen.numerical_reflection(std::to_string(varDepth));
                gen.fishermans_gambit();

                switch (term_un->op_type)
                {
                case TokenType::double_plus:
                    gen.numerical_reflection("1");
                    gen.additive_distilation();
                    break;
                case TokenType::double_dash:
                    gen.numerical_reflection("-1");
                    gen.additive_distilation();
                    break;
                }
                
                gen.numerical_reflection(std::to_string(-varDepth));
                gen.fishermans_gambit_II();
                return;
            }

            gen.gen_term(term_un->term);

            switch (term_un->op_type)
            {
            case TokenType::dash:
                gen.numerical_reflection("-1");
                gen.multiplicative_distilation();
                break;
            case TokenType::not_:
                gen.negation_purification();
                break;
            }
        }

        void operator()(const NodeTermUnPost* term_un_post)
        {
            const std::vector<Var>::iterator iter = std::find_if(gen.m_vars.begin(), gen.m_vars.end(),
                [&](const Var& var){ return var.name == term_un_post->ident.value.value(); });
            
            if (iter == gen.m_vars.end())
            {
                compilation_error(std::string("Undeclared identifier: ") + term_un_post->ident.value.value(), term_un_post->line);
            }

            Var& var = *iter;
            int varDepth = gen.m_stack_size - 1 - var.stack_loc;
            gen.numerical_reflection(std::to_string(varDepth));
            gen.fishermans_gambit();
            gen.gemini_decomposition();

            switch (term_un_post->op_type)
            {
            case TokenType::double_plus:
                gen.numerical_reflection("1");
                gen.additive_distilation();
                break;
            case TokenType::double_dash:
                gen.numerical_reflection("-1");
                gen.additive_distilation();
                break;
            }
            
            gen.numerical_reflection(std::to_string(-varDepth + 1));
            gen.fishermans_gambit();
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

            // Execute jump iota
            gen.jesters_gambit();
            gen.m_output << "Hermes' Gambit\n";
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
            gen.m_output << "{\n";
            gen.gen_stmt(stmt_if->stmt);
            gen.m_output << "}\n";
            gen.end_scope();

            // Potentially generate else statement
            if (stmt_if->else_stmt == nullptr)
            {
                gen.m_output << "Vacant Reflection\n";
            }
            else
            {
                gen.m_output << "{\n";
                gen.begin_scope();
                gen.gen_stmt(stmt_if->else_stmt);
                gen.end_scope(); 
                gen.m_output << "}\n";
            }
            
            // Perform bool comparison and execute
            gen.m_output << "Augur's Exaltation\n";
            gen.m_output << "Hermes' Gambit\n";
        }

        void operator()(const NodeStmtWhile* stmt_while)
        {
            gen.m_output << "{\n";
            ++gen.m_stack_size;
            gen.gen_expr(stmt_while->expr);

            // Exit loop if false
            gen.augurs_purification();
            gen.m_output << "Vacant Reflection\n";
            gen.m_output << "{\n";
            gen.m_output << "Charon's Gambit\n";
            gen.m_output << "}\n";
            gen.m_output << "Flock's Disintegration\n";
            gen.m_output << "Augur's Exaltation\n";
            gen.m_output << "Hermes' Gambit\n";
            
            // Generate statement
            --gen.m_stack_size;
            gen.begin_scope();
            gen.gen_stmt(stmt_while->stmt);
            gen.end_scope();

            // Execute next loop
            gen.m_output << "Gemini Decomposition\n";
            gen.m_output << "Hermes' Gambit\n";
            gen.m_output << "}\n";

            gen.m_output << "Gemini Decomposition\n";
            gen.m_output << "Hermes' Gambit\n";
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

    m_output << "{\n";
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

    end_scope();
    m_output << "}\n";

    // Account for function now being on the stack
    ++m_stack_size;

    generating_void_function = false;
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
        m_output << "Flock's Gambit\n";
        m_stack_size -= m_global_vars.size() + m_funcs.size();
    }
    else
    {
        vacant_reflection();
    }
    huginns_gambit();

    // Gen main
    for (NodeStmt* stmt : m_prog->main_->scope->stmts)
    {
        gen_stmt(stmt);
    }
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

void Generator::pop(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    m_output << "Bookkeeper's Gambit: " << std::string(amount, 'v') << '\n';
    m_stack_size -= amount;
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
    m_output << "Bookkeeper's Gambit: " << std::string(m_function_num_params, 'v') << '-' << std::string(pop_count - m_function_num_params, 'v') << (has_ret_value ? "-" : "") << '\n';
}

void Generator::dec_func(bool is_void, std::string name, int num_params, size_t line)
{
    if (std::find_if(m_funcs.cbegin(), m_funcs.cend(), [&](const Func& func){return func.name == name && func.num_params == num_params;}) != m_funcs.cend())
    {
        compilation_error(std::string("Function with this name and number of parameters already declared: ") + name, line);
    }

    m_funcs.push_back(Func{.is_void = is_void, .name = name, .num_params = num_params, .stack_loc = m_global_vars.size() + m_funcs.size()});
}



void Generator::additive_distilation()
{
    m_output << "Additive Distillation\n";
    --m_stack_size;
}

void Generator::alidades_purification()
{
    m_output << "Alidade's Purification\n";
}

void Generator::archers_distilation()
{
    m_output << "Archer's Distillation\n";
    --m_stack_size;
}

void Generator::architects_distilation()
{
    m_output << "Architect's Distillation\n";
    --m_stack_size;
}

void Generator::augurs_purification()
{
    m_output << "Augur's Purification\n";
}

void Generator::break_block()
{
    m_output << "Break Block\n";
    --m_stack_size;
}

void Generator::compass_purification()
{
    m_output << "Compass' Purification\n";
}

void Generator::compass_purification_II()
{
    m_output << "Compass' Purification II\n";
}

void Generator::conjunction_distilation()
{
    m_output << "Conjunction Distillation\n";
    --m_stack_size;
}

void Generator::conjure_light()
{
    m_output << "Conjure Light\n";
    --m_stack_size;
}

void Generator::disjunction_distilation()
{
    m_output << "Disjunction Distillation\n";
    --m_stack_size;
}

void Generator::division_distilation()
{
    m_output << "Division Distillation\n";
    --m_stack_size;
}

void Generator::equality_distilation()
{
    m_output << "Equality Distillation\n";
    --m_stack_size;
}

void Generator::false_reflection()
{
    m_output << "False Reflection\n";
    ++m_stack_size;
}

void Generator::fishermans_gambit()
{
    m_output << "Fisherman's Gambit\n";
    --m_stack_size;
}

void Generator::fishermans_gambit_II()
{
    m_output << "Fisherman's Gambit II\n";
}

void Generator::flocks_reflection()
{
    m_output << "Flock's Reflection\n";
    ++m_stack_size;
}

void Generator::gemini_decomposition()
{
    m_output << "Gemini Decomposition\n";
    ++m_stack_size;
}

void Generator::huginns_gambit()
{
    m_output << "Huginn's Gambit\n";
    --m_stack_size;
}

void Generator::inequality_distilation()
{
    m_output << "Inequality Distillation\n";
    --m_stack_size;
}

void Generator::jesters_gambit()
{
    m_output << "Jester's Gambit\n";
}

void Generator::maximus_distilation()
{
    m_output << "Maximus Distillation\n";
    --m_stack_size;
}

void Generator::maximus_distilation_II()
{
    m_output << "Maximus Distillation II\n";
    --m_stack_size;
}

void Generator::minds_reflection()
{
    m_output << "Mind's Reflection\n";
    ++m_stack_size;
}

void Generator::minimus_distilation()
{
    m_output << "Minimus Distillation\n";
    --m_stack_size;
}

void Generator::minimus_distilation_II()
{
    m_output << "Minimus Distillation II\n";
    --m_stack_size;
}

void Generator::modulus_distilation()
{
    m_output << "Modulus Distillation\n";
    --m_stack_size;
}

void Generator::multiplicative_distilation()
{
    m_output << "Multiplicative Distillation\n";
    --m_stack_size;
}

void Generator::muninns_reflection()
{
    m_output << "Muninn's Reflection\n";
    ++m_stack_size;
}

void Generator::negation_purification()
{
    m_output << "Negation Purification\n";
}

void Generator::nullary_reflection()
{
    m_output << "Nullary Reflection\n";
    ++m_stack_size;
}

void Generator::numerical_reflection(std::string value)
{
    m_output << "Numerical Reflection: " << value << '\n';
    ++m_stack_size;
}

void Generator::power_distilation()
{
    m_output << "Power Distillation\n";
    --m_stack_size;
}

void Generator::reveal()
{
    m_output << "Reveal\n";
}

void Generator::selection_distilation()
{
    m_output << "Selection Distillation\n";
    --m_stack_size;
}

void Generator::subtractive_distilation()
{
    m_output << "Subtractive Distillation\n";
    --m_stack_size;
}

void Generator::surgeons_exaltation()
{
    m_output << "Surgeon's Exaltation\n";
    m_stack_size -= 2;
}

void Generator::true_reflection()
{
    m_output << "True Reflection\n";
    ++m_stack_size;
}

void Generator::vacant_reflection()
{
    m_output << "Vacant Reflection\n";
    ++m_stack_size;
}

void Generator::vector_exaltation()
{
    m_output << "Vector Exaltation\n";
    m_stack_size -= 2;
}