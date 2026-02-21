#include "generation.hpp"

#include <sstream>

#include "util.hpp"
#include "inbuiltfuncs.hpp"

Generator::Generator(const NodeProg* prog)
    :m_prog(prog)
{ }

std::vector<Pattern> Generator::generate()
{
    gen_prog();

    return m_output;
}

void Generator::gen_assignment(const NodeTermVar* term_var, const std::variant<const NodeExpr*, const float> value, TokenType_ op, size_t line, bool is_post)
{
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

    // Extract var info
    struct VarVisitor {
        Generator& gen;
        bool& is_subscript;
        std::string& ident_name;
        NodeExpr*& subscript_expr;
        VarVisitor (Generator& _gen, bool& _is_subscript, std::string& _ident_name, NodeExpr*& _subscript_expr)
            :gen(_gen), is_subscript(_is_subscript), ident_name(_ident_name), subscript_expr(_subscript_expr) {}
        
        void operator()(const NodeVarIdent* ident)
        {
            is_subscript = false;
            ident_name = ident->ident.value.value();
        }

        void operator()(const NodeVarListSubscript* list)
        {
            is_subscript = true;
            ident_name = list->ident.value.value();
            subscript_expr = list->expr;
        }
    };

    bool is_subscript;
    std::string ident_name;
    NodeExpr* subscript_expr = nullptr;

    VarVisitor varVisitor(*this, is_subscript, ident_name, subscript_expr);
    std::visit(varVisitor, term_var->var);

    Var var = gen_var_ident(ident_name, term_var->line, op == TokenType_::eq && !is_subscript, false);

    // Gen index and handle list
    if (is_subscript)
    {
        gen_expr(subscript_expr);

        // If it's not eq, grab relavent list item
        if (op != TokenType_::eq)
        {
            // Duplicate list and index so we can re-assign it later
            dioscuri_gambit();
            selection_distillation();
            numerical_reflection("3");
            fishermans_gambit();
            jesters_gambit();
        }
        else
        {
            rotation_gambit();
        }
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
        if (!var.is_global && !is_subscript)
        {
            pop();
        }
        break;
    case TokenType_::plus_eq:
        additive_distillation();
        break;
    case TokenType_::dash_eq:
        jesters_gambit();
        subtractive_distillation();
        break;
    case TokenType_::star_eq:
        multiplicative_distillation();
        break;
    case TokenType_::fslash_eq:
        jesters_gambit();
        division_distillation();
        break;
    case TokenType_::mod_eq:
        jesters_gambit();
        modulus_distillation();
        break;
    }

    // Place value in list
    if (is_subscript)
    {
        surgeons_exaltation();
    }
    
    // If local
    if (!var.is_global)
    {
        numerical_reflection(std::to_string(-(int)m_stack_size + (int)var.stack_loc + 1));

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

bool Generator::gen_inbuilt_func(const NodeDefinedFunc* func, bool is_void, bool is_member)
{
    // Find inbuilt function
    std::vector<InbuiltFunc>::iterator func_it = std::find_if(inbuilt_funcs.begin(), inbuilt_funcs.end(), [&](const InbuiltFunc& curr){
        // Check if the current function matches the inbuilt function we're looking for (matches ret type, membership, number of params, and name)
        return curr.is_void == is_void && curr.is_member == is_member && curr.num_params == func->exprs.size() &&
                std::find(curr.names.cbegin(), curr.names.cend(), func->ident.value.value()) != curr.names.cend();
    });

    // If it doesn't exist, then there is no inbuilt function matching the description
    if (func_it == inbuilt_funcs.end())
    {
        return false;
    }

    // Generate expressions to place the values on top of the stack
    for (NodeExpr* expr : func->exprs)
    {
        gen_expr(expr);
    }

    // Add patterns for inbuilt func to prog
    for (Pattern p : func_it->get_patterns())
    {
        m_output.push_back(p);
    }

    // Fix stack size by removing generated expressions, removing member expression if there is one, and adding return expression if there is one
    m_stack_size += -func->exprs.size() - (is_member ? 1 : 0) + (is_void ? 0 : 1);

    return true;
}

bool Generator::gen_call_func(const NodeDefinedFunc* func)
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
    selection_distillation();
    // Iris' Gambit
    add_pattern(PatternType::iris_gambit, 0);

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
        if (!std::holds_alternative<NodeTermVar*>(term->var))
        {
            compilation_error("Expected identifier", expr_bin->line);
        }

        gen_assignment(std::get<NodeTermVar*>(term->var), expr_bin->rhs, expr_bin->op_type, expr_bin->line);

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
            if (std::holds_alternative<NodeTermCallFunc*>(term->var))
            {
                NodeTermCallFunc* term_func = std::get<NodeTermCallFunc*>(term->var);

                if (!gen_inbuilt_func(term_func->func, false, true))
                {
                    compilation_error("Expected member function", expr_bin->line);
                }
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
        equality_distillation();
        break;
    case TokenType_::not_eq_:
        inequality_distillation();
        break;
    case TokenType_::angle_open:
        minimus_distillation();
        break;
    case TokenType_::oangle_eq:
        minimus_distillation_II();
        break;
    case TokenType_::angle_close:
        maximus_distillation();
        break;
    case TokenType_::cangle_eq:
        maximus_distillation_II();
        break;
    case TokenType_::plus:
        additive_distillation();
        break;
    case TokenType_::dash:
        subtractive_distillation(); 
        break;
    case TokenType_::star:
        multiplicative_distillation();
        break;
    case TokenType_::slash_forward:
        division_distillation();
        break;
    case TokenType_::modulus:
        modulus_distillation();
        break;
    case TokenType_::double_amp:
        conjunction_distillation();
        break;
    case TokenType_::double_bar:
        disjunction_distillation();
        break;
    case TokenType_::caret:
        exclusion_distillation();
        break;
    }
}

Generator::Var Generator::gen_var_ident(const std::string ident_name, size_t line, bool dont_gen_if_global, bool leave_copy)
{
    std::vector<Var>::iterator iter = std::find_if(m_vars.begin(), m_vars.end(),
        [&](const Var& var){ return var.name == ident_name; });
    
    if (iter == m_vars.end())
    {
        iter = std::find_if(m_global_vars.begin(), m_global_vars.end(),
            [&](const Var& var){ return var.name == ident_name; });
        
        if (iter == m_global_vars.end())
        {
            compilation_error(std::string("Undeclared identifier: ") + ident_name, line);
        }
    }

    Var& var = *iter;

    if (var.is_global)
    {
        if (!dont_gen_if_global)
        {
            muninns_reflection();
            numerical_reflection(std::to_string(var.stack_loc));
            selection_distillation();
        }
    }
    else
    {
        numerical_reflection(std::to_string((int)m_stack_size - 1 - (int)var.stack_loc));

        if (leave_copy)
        {
            fishermans_gambit_II();
        }
        else
        {
            fishermans_gambit();
        }
    }

    return var;
}

void Generator::gen_var(const NodeTermVar* var)
{
    struct VarVisitor {
        Generator& gen;
        VarVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeVarIdent* var_ident)
        {
            gen.gen_var_ident(var_ident->ident.value.value(), var_ident->line);
        }

        void operator()(const NodeVarListSubscript* var_list)
        {
            gen.gen_var_ident(var_list->ident.value.value(), var_list->line);
            gen.gen_expr(var_list->expr);
            gen.selection_distillation();
        }
    };

    VarVisitor visitor(*this);
    std::visit(visitor, var->var);
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
                // Check to make sure term is a var
                if (!std::holds_alternative<NodeTermVar*>(term_un->term->var))
                {
                    compilation_error("Expected identifier", term_un->line);
                }

                gen.gen_assignment(std::get<NodeTermVar*>(term_un->term->var), (float)((term_un->op_type == TokenType_::double_plus) ? 1 : -1), TokenType_::plus_eq, term_un->line);

                return;
            }

            gen.gen_term(term_un->term);

            switch (term_un->op_type)
            {
            case TokenType_::dash:
                gen.numerical_reflection("-1");
                gen.multiplicative_distillation();
                break;
            case TokenType_::tilde:
            case TokenType_::not_:
                gen.negation_purification();
                break;
            }
        }

        void operator()(const NodeTermUnPost* term_un_post)
        {
            gen.gen_assignment(term_un_post->vari, (float)((term_un_post->op_type == TokenType_::double_plus) ? 1 : -1), TokenType_::plus_eq, term_un_post->line, true);
        }

        void operator()(const NodeTermNumLit* term_num_lit)
        {
            gen.add_embedded_iota(term_num_lit->num_lit.value.value());
        }

        void operator()(const NodeTermListLit* term_list_lit)
        {
            if (term_list_lit->exprs.size() <= 0)
            {
                gen.vacant_reflection();
            }
            else
            {
                for (NodeExpr* expr : term_list_lit->exprs)
                {
                    gen.gen_expr(expr);
                }

                gen.numerical_reflection(std::to_string(term_list_lit->exprs.size()));
                gen.flocks_gambit(term_list_lit->exprs.size());
            }
        }

        void operator()(const NodeTermIotaLit* term_pattern_lit)
        {
            gen.add_embedded_iota(term_pattern_lit->pattern_lit.value.value());
        }

        void operator()(const NodeTermBoolLit* term_bool_lit)
        {
            if (term_bool_lit->bool_.value == "true")
            {
                gen.add_embedded_iota("True");
            }
            else
            {
                gen.add_embedded_iota("False");
            }
        }

        void operator()(const NodeTermNullLit* term_null_lit)
        {
            gen.add_embedded_iota("Null");
        }

        void operator()(const NodeTermVar* term_var)
        {
            gen.gen_var(term_var);
        }

        void operator()(const NodeTermParen* term_paren)
        {
            gen.gen_expr(term_paren->expr);
        }

        void operator()(const NodeTermCallFunc* call_func)
        {
            if (!gen.gen_inbuilt_func(call_func->func, false, false) && gen.gen_call_func(call_func->func))
            {
                compilation_error("Calling void function as non-void function", call_func->line);
            }
        }
    };

    TermVisitor visitor(*this);
    std::visit(visitor, term->var);
}

void Generator::gen_expr(const NodeExpr* expr)
{
    // Check if we can gen during compile-time
    Iota compile_time_result = gen_comp_time_expr(expr);
    if (compile_time_result.has_value)
    {
        add_pattern(PatternType::introspection, 0);
        m_output.push_back(compile_time_result.get_iota_pattern());
        add_pattern(PatternType::retrospection, 1);
        add_pattern(PatternType::flocks_disintegration, 0);
        return;
    }

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

        void operator()(const NodeStmtCallFunction* call_func)
        {
            // Try to gen void inbuilt func first, then non-void inbuilt func, then defined func
            if (!gen.gen_inbuilt_func(call_func->func, true, false))
            {
                if (gen.gen_inbuilt_func(call_func->func, false, false) || !gen.gen_call_func(call_func->func))
                {
                    gen.pop();
                }
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
            gen.add_pattern(PatternType::hermes_gambit, 0);
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
            gen.m_vars.push_back(Var{.name = stmt_let->ident.value.value(), .stack_loc = gen.m_stack_size - 1, .is_global = false});
        }

        void operator()(const NodeStmtIf* stmt_if)
        {
            // Evaluate expression
            gen.gen_expr(stmt_if->expr);
            gen.augurs_purification();
            --gen.m_stack_size;

            // Generate statement
            gen.add_pattern(PatternType::introspection, 0);
            gen.begin_scope();
            gen.gen_stmt(stmt_if->stmt);
            gen.end_scope();
            gen.add_pattern(PatternType::retrospection, 0);

            // Potentially generate else statement
            if (stmt_if->else_stmt == nullptr)
            {
                gen.vacant_reflection();
                --gen.m_stack_size;
            }
            else
            {
                gen.add_pattern(PatternType::introspection, 0);
                gen.begin_scope();
                gen.gen_stmt(stmt_if->else_stmt);
                gen.end_scope(); 
                gen.add_pattern(PatternType::retrospection, 0);
            }
            
            // Perform bool comparison and execute
            gen.add_pattern(PatternType::augurs_exaltation, 0);
            gen.add_pattern(PatternType::hermes_gambit, 0);
            int (*test)(int) = [](int a) {return a;};
        }

        void operator()(const NodeStmtWhile* stmt_while)
        {
            // Add jump iota to stack for loop
            gen.vacant_reflection();
            gen.add_pattern(PatternType::iris_gambit, 0);

            // Gen condition
            gen.gen_expr(stmt_while->expr);

            // Account for condition not being on stack when generating loop body
            --gen.m_stack_size;

            // If true, gen statements and loop
            gen.add_pattern(PatternType::introspection, 0);
            gen.begin_scope();
            gen.gen_stmt(stmt_while->stmt);
            gen.end_scope();
            gen.gemini_decomposition();
            gen.add_pattern(PatternType::hermes_gambit, 0);
            gen.add_pattern(PatternType::retrospection, 0);

            // If false, do nothing
            gen.vacant_reflection();

            // Actually make comparison and execute
            gen.add_pattern(PatternType::augurs_exaltation, -2);
            gen.add_pattern(PatternType::hermes_gambit, 0);

            // Remove leftover jump iota from stack
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

    add_pattern(PatternType::introspection, 0);

    // Get jump iota to function, and exit function
    add_pattern(PatternType::introspection, 0);
    add_pattern(PatternType::charons_gambit, 0);
    add_pattern(PatternType::retrospection, 0);
    add_pattern(PatternType::flocks_disintegration, 0);
    add_pattern(PatternType::iris_gambit, 0);

    begin_scope();

    // Treat top of the stack as params
    for (Token param : params)
    {
        m_vars.push_back(Var{.name = param.value.value(), .stack_loc = m_stack_size, .is_global = false});
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
        // Pop scope items, but not jump iota since it will be used for return
        end_scopes_return(false);
        // Return
        add_pattern(PatternType::hermes_gambit, -1);
        // Remove local vars
        m_vars.resize(m_scopes.back().var_num);
        // Pop scope
        m_scopes.pop_back();
    }
    else
    {
        // Remove excess from stack
        end_scopes_return(false);
        // Add null to stack
        nullary_reflection();
        // Swap jump iota forward
        jesters_gambit();
        // Return
        add_pattern(PatternType::hermes_gambit, -1);
        // Remove local vars
        m_vars.resize(m_scopes.back().var_num);
        // Pop scope
        m_scopes.pop_back();
    }
    add_pattern(PatternType::retrospection, 1);
    add_pattern(PatternType::hermes_gambit, 0);
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
        m_vars.push_back(Var{.name = global_let->ident.value.value(), .stack_loc = m_vars.size(), .is_global = false});
    }

    // Clear temp local vars
    m_vars.clear();

    // Mark global variables as declared
    for (NodeGlobalLet* global_let : m_prog->vars)
    {
        m_global_vars.push_back(Var{.name = global_let->ident.value.value(), .stack_loc = m_global_vars.size(), .is_global = true});
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

    // Store functions and global vars in list in raven's mind
    if (m_global_vars.size() + m_funcs.size() > 0)
    {
        if (m_global_vars.size() + m_funcs.size() == 1)
        {
            singles_purification();
        }
        else
        {
            numerical_reflection(std::to_string(m_global_vars.size() + m_funcs.size()));
            flocks_gambit(m_global_vars.size() + m_funcs.size());
        }

        huginns_gambit();
    }

    // Remove accounting of jump iota so it isn't double counted
    --m_stack_size;

    // Gen main
    gen_func_def(m_prog->main_);

    // Execute main
    add_pattern(PatternType::iris_gambit, -1);
}



Iota Generator::gen_comp_time_expr(const NodeExpr* expr)
{
    // Visitor to generate compile time expression
    struct CompileTimeExprVisitor {
        Generator& gen;
        Iota result;
        CompileTimeExprVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeTerm* term)
        {
            result = gen.gen_comp_time_term(term);
        }

        void operator()(const NodeExprBin* expr_bin)
        {
            result = gen.gen_comp_time_bin_expr(expr_bin);
        }
    };

    CompileTimeExprVisitor visitor(*this);
    std::visit(visitor, expr->var);
    return visitor.result;
}

Iota Generator::gen_comp_time_term(const NodeTerm* term)
{
    // Visitor to generate compile time term
    struct CompileTimeTermVisitor {
        Generator& gen;
        Iota result;
        CompileTimeTermVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeTermUn* term_un)
        {
            // If unary expression is a type of assignment, can't be generated at compile time
            if (term_un->op_type == TokenType_::double_plus || term_un->op_type == TokenType_::double_dash)
            {
                result = Iota();
                return;
            }

            Iota term_result = gen.gen_comp_time_term(term_un->term);

            // If term can't be generated at compile time, then the whole expression can't be generated at compile time
            if (!term_result.has_value)
            {
                result = Iota();
                return;
            }

            switch (term_un->op_type)
            {
            case TokenType_::dash:
                if (std::holds_alternative<double>(term_result.value))
                {
                    result = Iota(-std::get<double>(term_result.value));
                }
                else if (std::holds_alternative<std::vector<double>>(term_result.value))
                {
                    std::vector<double> negated_vec;
                    for (double num : std::get<std::vector<double>>(term_result.value))
                    {
                        negated_vec.push_back(-num);
                    }
                    result = Iota(negated_vec);
                }
                else
                {
                    result = Iota();
                }
                break;
            case TokenType_::tilde:
            case TokenType_::not_:
                if (std::holds_alternative<bool>(term_result.value))
                {
                    result = Iota(!std::get<bool>(term_result.value));
                }
                else if (std::holds_alternative<double>(term_result.value))
                {
                    double num = std::get<double>(term_result.value);

                    // Round num to nearest integer, up in halfway cases, since bitwise operations only work on integers
                    num = std::floor(num + 0.5);

                    // Negate number
                    num = -num;

                    // Subsctract 1 since ~n is equal to -n-1
                    num -= 1;

                    result = Iota(num);
                }
                else
                {
                    result = Iota();
                }
                break;
            }
        }

        void operator()(const NodeTermUnPost* term_un_post)
        {
            // Currently, all post-unary expressions are not compile-time evaluatable
            result = Iota();
        }

        void operator()(const NodeTermNumLit* term_num_lit)
        {
            result = Iota(std::stod(term_num_lit->num_lit.value.value()));
        }

        void operator()(const NodeTermListLit* term_list_lit)
        {
            std::vector<Iota> expr_results;
            for (NodeExpr* expr : term_list_lit->exprs)
            {
                Iota result = gen.gen_comp_time_expr(expr);

                if (result.has_value)
                {
                    expr_results.push_back(result);
                }
                // If iota can't be generated at compile time, then the whole list can't be generated at compile time
                else
                {
                    result = Iota();
                    return;
                }
            }

            result = Iota(expr_results);
        }

        void operator()(const NodeTermIotaLit* term_iota_lit)
        {
            result = Iota(term_iota_lit->pattern_lit.value.value());
        }

        void operator()(const NodeTermBoolLit* term_bool_lit)
        {
            result = Iota(term_bool_lit->bool_.value == "true");
        }

        void operator()(const NodeTermNullLit* term_null_lit)
        {
            result = Iota(std::monostate{});
        }

        void operator()(const NodeTermVar* term_var)
        {
            // For now, count as not compile-time evaluatable. May change in the future
            result = Iota();
        }

        void operator()(const NodeTermParen* term_paren)
        {
            result = gen.gen_comp_time_expr(term_paren->expr);
        }

        void operator()(const NodeTermCallFunc* call_func)
        {
            // Try to find inbuilt function being called
            std::vector<InbuiltFunc>::iterator func_it = std::find_if(inbuilt_funcs.begin(), inbuilt_funcs.end(), [&](const InbuiltFunc& curr){
                // Check if the current function matches the inbuilt function we're looking for (matches ret type, membership, number of params, and name)
                return curr.is_void == false && curr.is_member == false && curr.num_params == call_func->func->exprs.size() &&
                        std::find(curr.names.cbegin(), curr.names.cend(), call_func->func->ident.value.value()) != curr.names.cend();
            });

            // If it doesn't exist, then there is no inbuilt function matching the description, so count as not compile-time evaluatable
            if (func_it == inbuilt_funcs.end())
            {
                result = Iota();
                return;
            }

            // If function isn't compile-time evaluatable, then count as not compile-time evaluatable
            if (func_it->eval == nullptr)
            {
                result = Iota();
                return;
            }

            // Generate compile-time results for expressions
            std::vector<Iota> expr_results;
            for (NodeExpr* expr : call_func->func->exprs)
            {
                Iota expr_result = gen.gen_comp_time_expr(expr);

                if (expr_result.has_value)
                {
                    expr_results.push_back(expr_result);
                }
                // If any expression can't be generated at compile time, then the whole function call can't be generated at compile time
                else
                {
                    result = Iota();
                    return;
                }
            }

            // Get result from inbuilt function
            result = func_it->eval(expr_results);
        }
    };

    CompileTimeTermVisitor visitor(*this);
    std::visit(visitor, term->var);
    return visitor.result;
}

Iota Generator::gen_comp_time_bin_expr(const NodeExprBin* expr_bin)
{
    // If binary expression is a type of assignment, can't be generated at compile time
    if (expr_bin->op_type == TokenType_::eq || expr_bin->op_type == TokenType_::plus_eq || expr_bin->op_type == TokenType_::dash_eq || expr_bin->op_type == TokenType_::star_eq
         || expr_bin->op_type == TokenType_::fslash_eq || expr_bin->op_type == TokenType_::mod_eq)
    {
        return Iota();
    }

    Iota lhs_result = gen_comp_time_expr(expr_bin->lhs);

    // If left-hand side can't be generated at compile time, then the whole expression can't be generated at compile time
    if (!lhs_result.has_value)
    {
        return Iota();
    }

    // If binary expression is calling a member function
    if (expr_bin->op_type == TokenType_::dot)
    {
        // If rhs is a term
        if (std::holds_alternative<NodeTerm*>(expr_bin->rhs->var))
        {
            NodeTerm* term = std::get<NodeTerm*>(expr_bin->rhs->var);
            // If term is a function
            if (std::holds_alternative<NodeTermCallFunc*>(term->var))
            {
                NodeTermCallFunc* term_func = std::get<NodeTermCallFunc*>(term->var);

                // Try to find inbuilt member function being called
                std::vector<InbuiltFunc>::iterator func_it = std::find_if(inbuilt_funcs.begin(), inbuilt_funcs.end(), [&](const InbuiltFunc& curr){
                    // Check if the current function matches the inbuilt member function we're looking for (matches ret type, membership, number of params, and name)
                    return curr.is_void == false && curr.is_member == true && curr.num_params == term_func->func->exprs.size() + 1 &&
                            std::find(curr.names.cbegin(), curr.names.cend(), term_func->func->ident.value.value()) != curr.names.cend();
                });

                // If it doesn't exist, then there is no inbuilt member function matching the description, so count as not compile-time evaluatable
                if (func_it == inbuilt_funcs.end())
                {
                    return Iota();
                }

                // If function isn't compile-time evaluatable, then count as not compile-time evaluatable
                if (func_it->eval == nullptr)
                {
                    return Iota();
                }

                // Generate compile-time results for expressions, starting with left-hand side as the first expression since it's a member function
                std::vector<Iota> expr_results = {lhs_result};
                for (NodeExpr* expr : term_func->func->exprs)
                {
                    Iota expr_result = gen_comp_time_expr(expr);

                    if (expr_result.has_value)
                    {
                        expr_results.push_back(expr_result);
                    }
                    // If any expression can't be generated at compile time, then the whole function call can't be generated at compile time
                    else
                    {
                        return Iota();
                    }
                }

                // Get result from inbuilt function
                return func_it->eval(expr_results);
            }
            else
            {
                return Iota();
            }
        }
        else
        {
            return Iota();
        }
    }

    Iota rhs_result = gen_comp_time_expr(expr_bin->rhs);

    // If right-hand side can't be generated at compile time, then the whole expression can't be generated at compile time
    if (!rhs_result.has_value)
    {
        return Iota();
    }

    switch(expr_bin->op_type)
    {
    case TokenType_::double_eq:
        return Iota(lhs_result == rhs_result);
        break;
    case TokenType_::not_eq_:
        return Iota(lhs_result != rhs_result);
        break;
    case TokenType_::angle_open:
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            return Iota(std::get<double>(lhs_result.value) < std::get<double>(rhs_result.value));
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::oangle_eq:
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            return Iota(std::get<double>(lhs_result.value) <= std::get<double>(rhs_result.value));
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::angle_close:
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            return Iota(std::get<double>(lhs_result.value) > std::get<double>(rhs_result.value));
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::cangle_eq:
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            return Iota(std::get<double>(lhs_result.value) >= std::get<double>(rhs_result.value));
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::plus:
        // Add two numbers
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            return Iota(std::get<double>(lhs_result.value) + std::get<double>(rhs_result.value));
        }
        // Add two vectors element-wise
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> sum_vec;
            for (size_t i = 0; i < lhs_vec.size(); ++i)
            {
                sum_vec.push_back(lhs_vec[i] + rhs_vec[i]);
            }
            return Iota(sum_vec);
        }
        // Add number to each element of vector (lhs vec + rhs num)
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            double rhs_num = std::get<double>(rhs_result.value);

            std::vector<double> sum_vec;
            for (size_t i = 0; i < lhs_vec.size(); ++i)
            {
                sum_vec.push_back(lhs_vec[i] + rhs_num);
            }
            return Iota(sum_vec);
        }
        // Add number to each element of vector (lhs num + rhs vec)
        else if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            double lhs_num = std::get<double>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> sum_vec;
            for (size_t i = 0; i < rhs_vec.size(); ++i)
            {
                sum_vec.push_back(lhs_num + rhs_vec[i]);
            }
            return Iota(sum_vec);
        }
        else if (std::holds_alternative<std::vector<Iota>>(lhs_result.value) && std::holds_alternative<std::vector<Iota>>(rhs_result.value))
        {
            const std::vector<Iota>& lhs_list = std::get<std::vector<Iota>>(lhs_result.value);
            const std::vector<Iota>& rhs_list = std::get<std::vector<Iota>>(rhs_result.value);

            std::vector<Iota> concat_list = lhs_list;
            concat_list.insert(concat_list.end(), rhs_list.begin(), rhs_list.end());

            return Iota(concat_list);
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::dash:
        // If both sides are numbers, subtract them
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            return Iota(std::get<double>(lhs_result.value) - std::get<double>(rhs_result.value));
        }
        // If both sides are vectors, subtract them element-wise
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> diff_vec;
            for (size_t i = 0; i < lhs_vec.size(); ++i)
            {
                diff_vec.push_back(lhs_vec[i] - rhs_vec[i]);
            }
            return Iota(diff_vec);
        }
        // If one side is a vector and the other side is a number, subtract the number from each element of the vector (lhs vec - rhs num)
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            double rhs_num = std::get<double>(rhs_result.value);

            std::vector<double> diff_vec;
            for (size_t i = 0; i < lhs_vec.size(); ++i)
            {
                diff_vec.push_back(lhs_vec[i] - rhs_num);
            }
            return Iota(diff_vec);
        }
        // If one side is a vector and the other side is a number, subtract each element of the vector from the number (lhs num - rhs vec)
        else if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            double lhs_num = std::get<double>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> diff_vec;
            for (size_t i = 0; i < rhs_vec.size(); ++i)
            {
                diff_vec.push_back(lhs_num - rhs_vec[i]);
            }
            return Iota(diff_vec);
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::star:
        // If both sides are numbers, multiply them
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            return Iota(std::get<double>(lhs_result.value) * std::get<double>(rhs_result.value));
        }
        // If one side is a vector and the other side is a number, multiply each element of the vector by the number (lhs vec * rhs num)
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            double rhs_num = std::get<double>(rhs_result.value);

            std::vector<double> prod_vec;
            for (size_t i = 0; i < lhs_vec.size(); ++i)
            {
                prod_vec.push_back(lhs_vec[i] * rhs_num);
            }
            return Iota(prod_vec);
        }
        // If one side is a vector and the other side is a number, multiply each element of the vector by the number (lhs num * rhs vec)
        else if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            double lhs_num = std::get<double>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> prod_vec;
            for (size_t i = 0; i < rhs_vec.size(); ++i)
            {
                prod_vec.push_back(lhs_num * rhs_vec[i]);
            }
            return Iota(prod_vec);
        }
        // If both sides are vectors, take their dot product
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            return Iota(std::inner_product(lhs_vec.begin(), lhs_vec.end(), rhs_vec.begin(), 0.0));
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::slash_forward:
        // If both sides are numbers, divide them
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            double rhs_num = std::get<double>(rhs_result.value);
            if (rhs_num == 0)
            {
                return Iota();
            }
            return Iota(std::get<double>(lhs_result.value) / rhs_num);
        }
        // If one side is a vector and the other side is a number, divide each element of the vector by the number (lhs vec / rhs num)
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            double rhs_num = std::get<double>(rhs_result.value);
            if (rhs_num == 0)
            {
                return Iota();
            }

            std::vector<double> quot_vec;
            for (size_t i = 0; i < lhs_vec.size(); ++i)
            {
                quot_vec.push_back(lhs_vec[i] / rhs_num);
            }
            return Iota(quot_vec);
        }
        // If one side is a vector and the other side is a number, divide each element of the vector by the number (lhs num / rhs vec)
        else if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            double lhs_num = std::get<double>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> quot_vec;
            for (size_t i = 0; i < rhs_vec.size(); ++i)
            {
                double rhs_elem = rhs_vec[i];
                if (rhs_elem == 0)
                {
                    return Iota();
                }
                quot_vec.push_back(lhs_num / rhs_elem);
            }
            return Iota(quot_vec);
        }
        // If both sides are vectors, take their cross product
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> cross_prod_vec = {
                lhs_vec[1] * rhs_vec[2] - lhs_vec[2] * rhs_vec[1],
                lhs_vec[2] * rhs_vec[0] - lhs_vec[0] * rhs_vec[2],
                lhs_vec[0] * rhs_vec[1] - lhs_vec[1] * rhs_vec[0]
            };

            return Iota(cross_prod_vec);
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::modulus:
        // If both sides are numbers, take modulus
        if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            double rhs_num = std::get<double>(rhs_result.value);

            if (rhs_num == 0)
            {
                return Iota();
            }

            double lhs_num = std::get<double>(lhs_result.value);

            float sign = lhs_num >= 0 ? 1 : -1;

            return Iota(sign * std::fmod(std::abs(lhs_num), std::abs(rhs_num)));
        }
        // If both sides are vectors, take modulus element-wise
        else if (std::holds_alternative<std::vector<double>>(lhs_result.value) && std::holds_alternative<std::vector<double>>(rhs_result.value))
        {
            const std::vector<double>& lhs_vec = std::get<std::vector<double>>(lhs_result.value);
            const std::vector<double>& rhs_vec = std::get<std::vector<double>>(rhs_result.value);

            std::vector<double> mod_vec;
            for (size_t i = 0; i < lhs_vec.size(); ++i)
            {
                double rhs_elem = rhs_vec[i];

                if (rhs_elem == 0)
                {
                    return Iota();
                }

                double lhs_elem = lhs_vec[i];
                float sign = lhs_elem >= 0 ? 1 : -1;

                mod_vec.push_back(sign * std::fmod(std::abs(lhs_elem), std::abs(rhs_elem)));
            }

            return Iota(mod_vec);
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::double_amp:
        // If both sides are booleans, take logical AND
        if (std::holds_alternative<bool>(lhs_result.value) && std::holds_alternative<bool>(rhs_result.value))
        {
            return Iota(std::get<bool>(lhs_result.value) && std::get<bool>(rhs_result.value));
        }
        // If both sides are lists, take their intersection
        else if (std::holds_alternative<std::vector<Iota>>(lhs_result.value) && std::holds_alternative<std::vector<Iota>>(rhs_result.value))
        {
            const std::vector<Iota>& lhs_list = std::get<std::vector<Iota>>(lhs_result.value);
            const std::vector<Iota>& rhs_list = std::get<std::vector<Iota>>(rhs_result.value);

            std::vector<Iota> intersection_list;
            for (const Iota& lhs_elem : lhs_list)
            {
                if (std::find(rhs_list.begin(), rhs_list.end(), lhs_elem) != rhs_list.end())
                {
                    intersection_list.push_back(lhs_elem);
                }
            }

            return Iota(intersection_list);
        }
        // If both sides are numbers, take bitwise AND
        else if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            double lhs_num = std::get<double>(lhs_result.value);
            double rhs_num = std::get<double>(rhs_result.value);

            // Round numbers to nearest integer, up in halfway cases, since bitwise operations only work on integers
            lhs_num = std::floor(lhs_num + 0.5);
            rhs_num = std::floor(rhs_num + 0.5);

            return Iota(static_cast<double>(static_cast<long long>(lhs_num) & static_cast<long long>(rhs_num)));
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::double_bar:
        // If both sides are booleans, take logical OR
        if (std::holds_alternative<bool>(lhs_result.value) && std::holds_alternative<bool>(rhs_result.value))
        {
            return Iota(std::get<bool>(lhs_result.value) || std::get<bool>(rhs_result.value));
        }
        // If both sides are lists, take their union
        else if (std::holds_alternative<std::vector<Iota>>(lhs_result.value) && std::holds_alternative<std::vector<Iota>>(rhs_result.value))
        {
            const std::vector<Iota>& lhs_list = std::get<std::vector<Iota>>(lhs_result.value);
            const std::vector<Iota>& rhs_list = std::get<std::vector<Iota>>(rhs_result.value);

            std::vector<Iota> union_list = lhs_list;

            for (const Iota& rhs_elem : rhs_list)
            {
                if (std::find(lhs_list.begin(), lhs_list.end(), rhs_elem) == lhs_list.end())
                {
                    union_list.push_back(rhs_elem);
                }
            }

            return Iota(union_list);
        }
        // If both sides are numbers, take bitwise OR
        else if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            double lhs_num = std::get<double>(lhs_result.value);
            double rhs_num = std::get<double>(rhs_result.value);

            // Round numbers to nearest integer, up in halfway cases, since bitwise operations only work on integers
            lhs_num = std::floor(lhs_num + 0.5);
            rhs_num = std::floor(rhs_num + 0.5);

            return Iota(static_cast<double>(static_cast<long long>(lhs_num) | static_cast<long long>(rhs_num)));
        }
        else
        {
            return Iota();
        }
        break;
    case TokenType_::caret:
        if (std::holds_alternative<bool>(lhs_result.value) && std::holds_alternative<bool>(rhs_result.value))
        {
            return Iota(std::get<bool>(lhs_result.value) != std::get<bool>(rhs_result.value));
        }
        else if (std::holds_alternative<std::vector<Iota>>(lhs_result.value) && std::holds_alternative<std::vector<Iota>>(rhs_result.value))
        {
            const std::vector<Iota>& lhs_list = std::get<std::vector<Iota>>(lhs_result.value);
            const std::vector<Iota>& rhs_list = std::get<std::vector<Iota>>(rhs_result.value);

            std::vector<Iota> sym_diff_list;

            for (const Iota& lhs_elem : lhs_list)
            {
                if (std::find(rhs_list.begin(), rhs_list.end(), lhs_elem) == rhs_list.end())
                {
                    sym_diff_list.push_back(lhs_elem);
                }
            }

            for (const Iota& rhs_elem : rhs_list)
            {
                if (std::find(lhs_list.begin(), lhs_list.end(), rhs_elem) == lhs_list.end())
                {
                    sym_diff_list.push_back(rhs_elem);
                }
            }

            return Iota(sym_diff_list);
        }
        else if (std::holds_alternative<double>(lhs_result.value) && std::holds_alternative<double>(rhs_result.value))
        {
            double lhs_num = std::get<double>(lhs_result.value);
            double rhs_num = std::get<double>(rhs_result.value);

            // Round numbers to nearest integer, up in halfway cases, since bitwise operations only work on integers
            lhs_num = std::floor(lhs_num + 0.5);
            rhs_num = std::floor(rhs_num + 0.5);

            return Iota(static_cast<double>(static_cast<long long>(lhs_num) ^ static_cast<long long>(rhs_num)));
        }
        else
        {
            return Iota();
        }
        break;
    default:
        return Iota();
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

    add_pattern(PatternType::bookkeepers_gambit, -amount, std::string(amount, 'v'));
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
    add_pattern(PatternType::bookkeepers_gambit, 0,
        // Preserve parameters if they exist
        (m_function_num_params != 0 ? (std::string(m_function_num_params, 'v') + '-') : std::string()) +
        // Pop rest of scope
        std::string(pop_count - m_function_num_params, 'v') +
        // Preserve return value if it exists
        (has_ret_value ? "-" : ""));
}

void Generator::dec_func(bool is_void, std::string name, int num_params, size_t line)
{
    if (std::find_if(m_funcs.cbegin(), m_funcs.cend(), [&](const Func& func){return func.name == name && func.num_params == num_params;}) != m_funcs.cend())
    {
        compilation_error(std::string("Function with this name and number of parameters already declared: ") + name, line);
    }

    m_funcs.push_back(Func{.is_void = is_void, .name = name, .num_params = num_params, .stack_loc = m_global_vars.size() + m_funcs.size()});
}



void Generator::akashas_distillation()
{
    add_pattern(PatternType::akashas_distillation, -1);
}

void Generator::akashas_gambit()
{
    add_pattern(PatternType::akashas_gambit, -3);
}

void Generator::additive_distillation()
{
    add_pattern(PatternType::additive_distillation, -1);
}

void Generator::anchorites_flight()
{
    add_pattern(PatternType::anchorites_flight, -2);
}

void Generator::alidades_purification()
{
    add_pattern(PatternType::alidades_purification, 0);
}

void Generator::aviators_purification()
{
    add_pattern(PatternType::aviators_purification, 0);
}

void Generator::archers_distillation()
{
    add_pattern(PatternType::archers_distillation, -1);
}

void Generator::architects_distillation()
{
    add_pattern(PatternType::architects_distillation, -1);
}

void Generator::arcs_reflection()
{
    add_pattern(PatternType::arcs_reflection, 1);
}

void Generator::assessors_purification()
{
    add_pattern(PatternType::assessors_purification, 0);
}

void Generator::assessors_reflection()
{
    add_pattern(PatternType::assessors_reflection, 1);
}

void Generator::auditors_purification()
{
    add_pattern(PatternType::auditors_purification, 0);
}

void Generator::auditors_reflection()
{
    add_pattern(PatternType::auditors_reflection, 1);
}

void Generator::augurs_exaltation()
{
    add_pattern(PatternType::augurs_exaltation, -2);
}

void Generator::augurs_purification()
{
    add_pattern(PatternType::augurs_purification, 0);
}

void Generator::axial_purification()
{
    add_pattern(PatternType::axial_purification, 0);
}

void Generator::black_suns_nadir()
{
    add_pattern(PatternType::black_suns_nadir, -3);
}

void Generator::black_suns_zenith()
{
    add_pattern(PatternType::black_suns_zenith, -3);
}

void Generator::blue_suns_nadir()
{
    add_pattern(PatternType::blue_suns_nadir, -2);
}

void Generator::blue_suns_zenith()
{
    add_pattern(PatternType::blue_suns_zenith, -2);
}

void Generator::bookkeepers_gambit(std::string value)
{
    add_pattern(PatternType::bookkeepers_gambit, -std::count(value.cbegin(), value.cend(), 'v'), value);
}

void Generator::break_block()
{
    add_pattern(PatternType::break_block, -1);
}

void Generator::ceiling_purification()
{
    add_pattern(PatternType::ceiling_purification, 0);
}

void Generator::chroniclers_gambit()
{
    add_pattern(PatternType::chroniclers_gambit, -2);
}

void Generator::chroniclers_prfn()
{
    add_pattern(PatternType::chroniclers_prfn, 0);
}

void Generator::chroniclers_purification()
{
    add_pattern(PatternType::chroniclers_prfn, 0);
}

void Generator::circle_reflection()
{
    add_pattern(PatternType::circle_reflection, 1);
}

void Generator::compass_purification()
{
    add_pattern(PatternType::compass_purification, 0);
}

void Generator::compass_purification_II()
{
    add_pattern(PatternType::compass_purification_II, 0);
}

void Generator::cosine_purification()
{
    add_pattern(PatternType::cosine_purification, 0);
}

void Generator::craft_phial()
{
    add_pattern(PatternType::craft_phial, -1);
}

void Generator::conjunction_distillation()
{
    add_pattern(PatternType::conjunction_distillation, -1);
}

void Generator::conjure_light()
{
    add_pattern(PatternType::conjure_light, -1);
}

void Generator::create_water()
{
    add_pattern(PatternType::create_water, -1);
}

void Generator::derivation_decomposition()
{
    add_pattern(PatternType::derivation_decomposition, 1);
}

void Generator::dioscuri_gambit()
{
    add_pattern(PatternType::dioscuri_gambit, 2);
}

void Generator::disjunction_distillation()
{
    add_pattern(PatternType::disjunction_distillation, -1);
}

void Generator::division_distillation()
{
    add_pattern(PatternType::division_distillation, -1);
}

void Generator::dispel_rain()
{
    add_pattern(PatternType::dispel_rain, 0);
}

void Generator::entropy_reflection()
{
    add_pattern(PatternType::entropy_reflection, 1);
}

void Generator::equality_distillation()
{
    add_pattern(PatternType::equality_distillation, -1);
}

void Generator::eulers_reflection()
{
    add_pattern(PatternType::eulers_reflection, 1);
}

void Generator::entity_prfn()
{
    add_pattern(PatternType::entity_prfn, 0);
}

void Generator::entity_prfn_animal()
{
    add_pattern(PatternType::entity_prfn_animal, 0);
}

void Generator::entity_prfn_item()
{
    add_pattern(PatternType::entity_prfn_item, 0);
}

void Generator::entity_prfn_living()
{
    add_pattern(PatternType::entity_prfn_living, 0);
}

void Generator::entity_prfn_monster()
{
    add_pattern(PatternType::entity_prfn_monster, 0);
}

void Generator::entity_prfn_player()
{
    add_pattern(PatternType::entity_prfn_player, 0);
}

void Generator::excisors_distillation()
{
    add_pattern(PatternType::excisors_distillation, -1);
}

void Generator::exclusion_distillation()
{
    add_pattern(PatternType::exclusion_distillation, -1);
}

void Generator::explosion()
{
    add_pattern(PatternType::explosion, -2);
}

void Generator::false_reflection()
{
    add_pattern(PatternType::false_reflection, 1);
}

void Generator::fireball()
{
    add_pattern(PatternType::fireball, -2);
}

void Generator::fishermans_gambit()
{
    add_pattern(PatternType::fishermans_gambit, -1);
}

void Generator::fishermans_gambit_II()
{
    add_pattern(PatternType::fishermans_gambit_II, 0);
}

void Generator::flocks_gambit(int num_iotas_packed)
{
    add_pattern(PatternType::flocks_gambit, -num_iotas_packed);
}

void Generator::flocks_reflection()
{
    add_pattern(PatternType::flocks_reflection, 1);
}

void Generator::floor_purification()
{
    add_pattern(PatternType::floor_purification, 0);
}

void Generator::gemini_decomposition()
{
    add_pattern(PatternType::gemini_decomposition, 1);
}

void Generator::greater_fold_reflection()
{
    add_pattern(PatternType::greater_fold_reflection, 1);
}

void Generator::huginns_gambit()
{
    add_pattern(PatternType::huginns_gambit, -1);
}

void Generator::inequality_distillation()
{
    add_pattern(PatternType::inequality_distillation, -1);
}

void Generator::integration_distillation()
{
    add_pattern(PatternType::integration_distillation, -1);
}

void Generator::inverse_cosine_purification()
{
    add_pattern(PatternType::inverse_cosine_purification, 0);
}

void Generator::inverse_sine_purification()
{
    add_pattern(PatternType::inverse_sine_purification, 0);
}

void Generator::inverse_tangent_distillation()
{
    add_pattern(PatternType::inverse_tangent_distillation, -1);
}

void Generator::inverse_tangent_purification()
{
    add_pattern(PatternType::inverse_tangent_purification, 0);
}

void Generator::jesters_gambit()
{
    add_pattern(PatternType::jesters_gambit, 0);
}

void Generator::length_purification()
{
    add_pattern(PatternType::length_purification, 0);
}

void Generator::lesser_fold_reflection()
{
    add_pattern(PatternType::lesser_fold_reflection, 1);
}

void Generator::locators_distillation()
{
    add_pattern(PatternType::locators_distillation, -1);
}

void Generator::lodestone_reflection()
{
    add_pattern(PatternType::lodestone_reflection, 1);
}

void Generator::logarithmic_distillation()
{
    add_pattern(PatternType::logarithmic_distillation, -1);
}

void Generator::make_note()
{
    add_pattern(PatternType::make_note, -3);
}

void Generator::maximus_distillation()
{
    add_pattern(PatternType::maximus_distillation, -1);
}

void Generator::maximus_distillation_II()
{
    add_pattern(PatternType::maximus_distillation_II, -1);
}

void Generator::minds_reflection()
{
    add_pattern(PatternType::minds_reflection, 1);
}

void Generator::minimus_distillation()
{
    add_pattern(PatternType::minimus_distillation, -1);
}

void Generator::minimus_distillation_II()
{
    add_pattern(PatternType::minimus_distillation_II, -1);
}

void Generator::modulus_distillation()
{
    add_pattern(PatternType::modulus_distillation, -1);
}

void Generator::multiplicative_distillation()
{
    add_pattern(PatternType::multiplicative_distillation, -1);
}

void Generator::muninns_reflection()
{
    add_pattern(PatternType::muninns_reflection, 1);
}

void Generator::negation_purification()
{
    add_pattern(PatternType::negation_purification, 0);
}

void Generator::nullary_reflection()
{
    add_pattern(PatternType::nullary_reflection, 1);
}

void Generator::numerical_reflection(std::string value)
{
    add_pattern(PatternType::numerical_reflection, 1, value);
}

void Generator::pace_purification()
{
    add_pattern(PatternType::pace_purification, 0);
}

void Generator::power_distillation()
{
    add_pattern(PatternType::power_distillation, -1);
}

void Generator::prospectors_gambit()
{
    add_pattern(PatternType::prospectors_gambit, 1);
}

void Generator::retrograde_purification()
{
    add_pattern(PatternType::retrograde_purification, 0);
}

void Generator::reveal()
{
    add_pattern(PatternType::reveal, 0);
}

void Generator::rotation_gambit()
{
    add_pattern(PatternType::rotation_gambit, 0);
}

void Generator::rotation_gambit_II()
{
    add_pattern(PatternType::rotation_gambit_II, 0);
}

void Generator::scouts_distillation()
{
    add_pattern(PatternType::scouts_distillation, -1);
}

void Generator::scribes_gambit()
{
    add_pattern(PatternType::scribes_gambit, -1);
}

void Generator::scribes_reflection()
{
    add_pattern(PatternType::scribes_reflection, 1);
}

void Generator::selection_distillation()
{
    add_pattern(PatternType::selection_distillation, -1);
}

void Generator::selection_exaltation()
{
    add_pattern(PatternType::selection_exaltation, -2);
}

void Generator::singles_purification()
{
    add_pattern(PatternType::singles_purification, 0);
}

void Generator::speakers_decomposition()
{
    add_pattern(PatternType::speakers_decomposition, 1);
}

void Generator::speakers_distillation()
{
    add_pattern(PatternType::speakers_distillation, -1);
}

void Generator::stadiometers_purification()
{
    add_pattern(PatternType::stadiometers_prfn, 0);
}

void Generator::subtractive_distillation()
{
    add_pattern(PatternType::subtractive_distillation, -1);
}

void Generator::sine_purification()
{
    add_pattern(PatternType::sine_purification, 0);
}

void Generator::stadiometers_prfn()
{
    add_pattern(PatternType::stadiometers_prfn, 0);
}

void Generator::swindlers_gambit()
{
    add_pattern(PatternType::swindlers_gambit, -1);
}

void Generator::summon_lightning()
{
    add_pattern(PatternType::summon_lightning, -1);
}

void Generator::summon_rain()
{
    add_pattern(PatternType::summon_rain, 0);
}

void Generator::surgeons_exaltation()
{
    add_pattern(PatternType::surgeons_exaltation, -2);
}

void Generator::tangent_purification()
{
    add_pattern(PatternType::tangent_purification, 0);
}

void Generator::thanatos_reflection()
{
    add_pattern(PatternType::thanatos_reflection, 1);
}

void Generator::true_reflection()
{
    add_pattern(PatternType::true_reflection, 1);
}

void Generator::uniqueness_purification()
{
    add_pattern(PatternType::uniqueness_purification, 0);
}

void Generator::vacant_reflection()
{
    // This uses no patterns but achieves the same effect as vacant reflection
    add_pattern(PatternType::introspection, 0);
    add_pattern(PatternType::retrospection, 1);
    //add_pattern(PatternType::vacant_reflection, 1);
}

void Generator::vector_disintegration()
{
    add_pattern(PatternType::vector_disintegration, 2);
}

void Generator::vector_exaltation()
{
    add_pattern(PatternType::vector_exaltation, -2);
}

void Generator::vector_reflection_NX()
{
    add_pattern(PatternType::vector_reflection_NX, 1);
}

void Generator::vector_reflection_NY()
{
    add_pattern(PatternType::vector_reflection_NY, 1);
}

void Generator::vector_reflection_NZ()
{
    add_pattern(PatternType::vector_reflection_NZ, 1);
}

void Generator::vector_reflection_PX()
{
    add_pattern(PatternType::vector_reflection_PX, 1);
}

void Generator::vector_reflection_PY()
{
    add_pattern(PatternType::vector_reflection_PY, 1);
}

void Generator::vector_reflection_PZ()
{
    add_pattern(PatternType::vector_reflection_PZ, 1);
}

void Generator::vector_reflection_zero()
{
    add_pattern(PatternType::vector_reflection_zero, 1);
}

void Generator::zone_dstl_animal()
{
    add_pattern(PatternType::zone_dstl_animal, -1);
}

void Generator::zone_dstl_any()
{
    add_pattern(PatternType::zone_dstl_any, -1);
}

void Generator::zone_dstl_item()
{
    add_pattern(PatternType::zone_dstl_item, -1);
}

void Generator::zone_dstl_living()
{
    add_pattern(PatternType::zone_dstl_living, -1);
}

void Generator::zone_dstl_monster()
{
    add_pattern(PatternType::zone_dstl_monster, -1);
}

void Generator::zone_dstl_non_animal()
{
    add_pattern(PatternType::zone_dstl_non_animal, -1);
}

void Generator::zone_dstl_non_item()
{
    add_pattern(PatternType::zone_dstl_non_item, -1);
}

void Generator::zone_dstl_non_living()
{
    add_pattern(PatternType::zone_dstl_non_living, -1);
}

void Generator::zone_dstl_non_monster()
{
    add_pattern(PatternType::zone_dstl_non_monster, -1);
}

void Generator::zone_dstl_non_player()
{
    add_pattern(PatternType::zone_dstl_non_player, -1);
}

void Generator::zone_dstl_player()
{
    add_pattern(PatternType::zone_dstl_player, -1);
}

void Generator::altiora()
{
    add_pattern(PatternType::altiora, -1);
}

void Generator::banish_sentinel()
{
    add_pattern(PatternType::banish_sentinel, 0);
}

void Generator::blink()
{
    add_pattern(PatternType::blink, -2);
}

void Generator::casters_glamour()
{
    add_pattern(PatternType::casters_glamour, 0);
}

void Generator::conjure_block()
{
    add_pattern(PatternType::conjure_block, -1);
}

void Generator::craft_artifact()
{
    add_pattern(PatternType::craft_artifact, -2);
}

void Generator::craft_cypher()
{
    add_pattern(PatternType::craft_cypher, -2);
}

void Generator::craft_trinket()
{
    add_pattern(PatternType::craft_trinket, -2);
}

void Generator::create_lava()
{
    add_pattern(PatternType::create_lava, -1);
}

void Generator::destroy_liquid()
{
    add_pattern(PatternType::destroy_liquid, -1);
}

void Generator::edify_sapling()
{
    add_pattern(PatternType::edify_sapling, -1);
}

void Generator::erase_item()
{
    add_pattern(PatternType::erase_item, 0);
}

void Generator::extinguish_area()
{
    add_pattern(PatternType::extinguish_area, -1);
}

void Generator::flay_mind()
{
    add_pattern(PatternType::flay_mind, -2);
}

void Generator::greater_teleport()
{
    add_pattern(PatternType::greater_teleport, -2);
}

void Generator::green_suns_nadir()
{
    add_pattern(PatternType::green_suns_nadir, -3);
}

void Generator::green_suns_zenith()
{
    add_pattern(PatternType::green_suns_zenith, -3);
}

void Generator::ignite()
{
    add_pattern(PatternType::ignite, -1);
}

void Generator::impulse()
{
    add_pattern(PatternType::impulse, -2);
}

void Generator::internalize_pigment()
{
    add_pattern(PatternType::internalize_pigment, 0);
}

void Generator::locate_sentinel()
{
    add_pattern(PatternType::locate_sentinel, 1);
}

void Generator::overgrow()
{
    add_pattern(PatternType::overgrow, -1);
}

void Generator::place_block()
{
    add_pattern(PatternType::place_block, -1);
}

void Generator::recharge_item()
{
    add_pattern(PatternType::recharge_item, -1);
}

void Generator::red_suns_nadir()
{
    add_pattern(PatternType::red_suns_nadir, -3);
}

void Generator::red_suns_zenith()
{
    add_pattern(PatternType::red_suns_zenith, -3);
}

void Generator::summon_greater_sentinel()
{
    add_pattern(PatternType::summon_greater_sentinel, -1);
}

void Generator::summon_sentinel()
{
    add_pattern(PatternType::summon_sentinel, -1);
}

void Generator::wayfind_sentinel()
{
    add_pattern(PatternType::wayfind_sentinel, 0);
}

void Generator::wayfarers_flight()
{
    add_pattern(PatternType::wayfarers_flight, -2);
}

void Generator::waystone_reflection()
{
    add_pattern(PatternType::waystone_reflection, 1);
}

void Generator::white_suns_nadir()
{
    add_pattern(PatternType::white_suns_nadir, -3);
}

void Generator::white_suns_zenith()
{
    add_pattern(PatternType::white_suns_zenith, -3);
}

void Generator::add_embedded_iota(std::string val)
{
    add_pattern(PatternType::introspection, 0);
    add_pattern(PatternType::embedded_iota, 0, val);
    add_pattern(PatternType::retrospection, 1);
    add_pattern(PatternType::flocks_disintegration, 0);
}

void Generator::add_pattern(PatternType pattern_type, size_t stack_size_net, std::optional<std::string> value)
{
    m_output.push_back(Pattern{.type = pattern_type, .value = value});
    m_stack_size += stack_size_net;
}