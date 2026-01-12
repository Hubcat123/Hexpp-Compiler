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
    case TokenType::block_normal_raycast:
        try_gen_x_exprs(func->exprs, 2, func->line);
        architects_distilation();
        break;
    case TokenType::summon_light:
        try_gen_x_exprs(func->exprs, 1, func->line);
        conjure_light();
        break;
    }
}

void Generator::gen_bin_expr(const NodeExprBin* expr_bin)
{
    // If binary expression is a type of assignment
    if (expr_bin->op_type == TokenType::eq || expr_bin->op_type == TokenType::plus_eq || expr_bin->op_type == TokenType::dash_eq || expr_bin->op_type == TokenType::star_eq
         || expr_bin->op_type == TokenType::fslash_eq || expr_bin->op_type == TokenType::mod_eq)
    {
        if (!expr_bin->ident.has_value())
        {
            compilation_error("Expected identifier", expr_bin->line);
        }

        const std::vector<Var>::iterator iter = std::find_if(m_vars.begin(), m_vars.end(),
            [&](const Var& var){ return var.name == expr_bin->ident.value().value.value(); });
        
        if (iter == m_vars.end())
        {
            compilation_error(std::string("Undeclared identifier: ") + expr_bin->ident.value().value.value(), expr_bin->line);
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

        void operator()(const NodeTermNumLit* term_int_lit)
        {
            gen.numerical_reflection(term_int_lit->num_lit.value.value());
        }

        void operator()(const NodeTermIdent* term_ident)
        {
            const std::vector<Var>::iterator iter = std::find_if(gen.m_vars.begin(), gen.m_vars.end(),
                [&](const Var& var){ return var.name == term_ident->ident.value.value(); });
            
            if (iter == gen.m_vars.end())
            {
                compilation_error(std::string("Undeclared identifier: ") + term_ident->ident.value.value(), term_ident->line);
            }

            Var& var = *iter;
            gen.numerical_reflection(std::to_string(gen.m_stack_size - 1 - var.stack_loc));
            gen.fishermans_gambit_II();
        }

        void operator()(const NodeTermParen* term_paren)
        {
            gen.gen_expr(term_paren->expr);
        }

        void operator()(const NodeTermFunc* term_func)
        {
            gen.gen_func(term_func->func);
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

        void operator()(const NodeExprFunc* expr_func)
        {
            gen.gen_expr(expr_func->expr);
            gen.gen_func(expr_func->func);
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

        void operator()(const NodeStmtFunc* stmt_func)
        {
            gen.gen_func(stmt_func->func);
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

void Generator::gen_prog()
{
    for (const NodeStmt* stmt : m_prog->stmts)
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
    m_scopes.push(Scope{.stack_size = m_stack_size, .var_num = m_vars.size()});
}

void Generator::end_scope()
{
    size_t pop_count = m_stack_size - m_scopes.top().stack_size;
    pop(pop_count);

    m_vars.resize(m_scopes.top().var_num);

    m_scopes.pop();
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

void Generator::negation_purification()
{
    m_output << "Negation Purification\n";
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
    m_output << "Reveal" << '\n';
}

void Generator::subtractive_distilation()
{
    m_output << "Subtractive Distillation\n";
    --m_stack_size;
}

void Generator::vector_exaltation()
{
    m_output << "Vector Exaltation\n";
    m_stack_size -= 2;
}