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
        try_gen_x_exprs(func->exprs, 1);
        reveal();
        pop();
        break;
    case TokenType::pow:
        try_gen_x_exprs(func->exprs, 2);
        power_distilation();
        break;
    }
}

void Generator::gen_bin_expr(const NodeExprBin* expr_bin)
{
    gen_expr(expr_bin->lhs);
    gen_expr(expr_bin->rhs);

    switch(expr_bin->op_type)
    {
    case TokenType::angle_open:
        minimus_distilation();
        break;
    case TokenType::angle_close:
        maximus_distilation();
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
    }
}

void Generator::gen_term(const NodeTerm* term)
{
    struct TermVisitor {
        Generator& gen;
        TermVisitor (Generator& _gen) :gen(_gen) {}
        
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
                compilation_error(std::string("Undeclared identifier: ") + term_ident->ident.value.value());
            }

            Var& var = *iter;
            gen.flocks_reflection();
            gen.numerical_reflection(std::to_string(var.stack_loc + 1));
            gen.subtractive_distilation();
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
        }

        void operator()(const NodeStmtLet* stmt_let)
        {
            if (std::find_if(gen.m_vars.cbegin(), gen.m_vars.cend(), [&](const Var& var){return var.name == stmt_let->ident.value.value();}) != gen.m_vars.cend())
            {
                compilation_error(std::string("Identifier already used: ") + stmt_let->ident.value.value());
            }

            gen.gen_expr(stmt_let->expr);
            gen.m_vars.push_back(Var{.name = stmt_let->ident.value.value(), .stack_loc = gen.m_stack_size - 1});
        }

        void operator()(const NodeStmtIf* stmt_if)
        {
            // Evaluate expression
            gen.gen_expr(stmt_if->expr);
            gen.augurs_purification();

            // Generate statement
            gen.m_output << "{\n";
            gen.gen_stmt(stmt_if->stmt);
            gen.m_output << "}\n";
            
            // Perform bool comparison and execute
            gen.m_output << "Vacant Reflection\n";
            gen.m_output << "Augur's Exaltation\n";
            gen.m_output << "Hermes' Gambit\n";
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



void Generator::try_gen_x_exprs(std::vector<NodeExpr*> exprs, int correct_amount)
{
    if (exprs.size() != correct_amount)
    {
        compilation_error("Incorrect number of arguments passed into function");
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
    m_scopes.push(m_vars.size());
}

void Generator::end_scope()
{
    size_t pop_count = m_vars.size() - m_scopes.top();
    pop(pop_count);

    m_vars.resize(m_scopes.top());

    m_scopes.pop();
}



void Generator::additive_distilation()
{
    m_output << "Additive Distillation\n";
    --m_stack_size;
}

void Generator::augurs_purification()
{
    m_output << "Augur's Purification\n";
}

void Generator::division_distilation()
{
    m_output << "Division Distillation\n";
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

void Generator::maximus_distilation()
{
    m_output << "Maximus Distillation\n";
    --m_stack_size;
}

void Generator::minimus_distilation()
{
    m_output << "Minimus Distillation\n";
    --m_stack_size;
}

void Generator::multiplicative_distilation()
{
    m_output << "Multiplicative Distillation\n";
    --m_stack_size;
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