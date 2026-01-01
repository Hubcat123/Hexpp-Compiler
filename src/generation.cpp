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

void Generator::gen_bin_expr(const NodeExprBin* expr_bin)
{
    struct ExprBinVisitor {
        Generator& gen;
        ExprBinVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeExprAdd* expr_add)
        {
            gen.gen_expr(expr_add->lhs);
            gen.gen_expr(expr_add->rhs);
            gen.additive_distilation();
        }

        void operator()(const NodeExprMulti* expr_multi)
        {
            gen.gen_expr(expr_multi->lhs);
            gen.gen_expr(expr_multi->rhs);
            gen.multiplicative_distilation();
        }
    };

    ExprBinVisitor visitor(*this);
    std::visit(visitor, expr_bin->var);
}

void Generator::gen_term(const NodeTerm* term)
{
    struct TermVisitor {
        Generator& gen;
        TermVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeTermNumLit* expr_int_lit)
        {
            gen.numerical_reflection(expr_int_lit->num_lit.value.value());
        }

        void operator()(const NodeTermIdent* expr_ident)
        {
            if (!gen.m_vars.contains(expr_ident->ident.value.value()))
            {
                compilation_error(std::string("Undeclared identifier: ") + expr_ident->ident.value.value());
            }

            Var& var = gen.m_vars.at(expr_ident->ident.value.value());
            gen.numerical_reflection(std::to_string(gen.m_stack_size - var.stack_loc - 1));
            gen.fishermans_gambit_II();
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
        
        void operator()(const NodeTermNumLit* expr_int_lit)
        {
            gen.numerical_reflection(expr_int_lit->num_lit.value.value());
        }

        void operator()(const NodeTermIdent* expr_ident)
        {
            if (!gen.m_vars.contains(expr_ident->ident.value.value()))
            {
                compilation_error(std::string("Undeclared identifier: ") + expr_ident->ident.value.value());
            }

            Var& var = gen.m_vars.at(expr_ident->ident.value.value());
            gen.numerical_reflection(std::to_string(gen.m_stack_size - var.stack_loc - 1));
            gen.fishermans_gambit_II();
        }

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

        void operator()(const NodeStmtPrint* stmt_print)
        {
            gen.gen_expr(stmt_print->expr);
            gen.reveal();
            gen.pop();
        }

        void operator()(const NodeStmtLet* stmt_let)
        {
            if (gen.m_vars.contains(stmt_let->ident.value.value()))
            {
                compilation_error(std::string("Identifier already used: ") + stmt_let->ident.value.value());
            }

            gen.gen_expr(stmt_let->expr);
            gen.m_vars.insert({stmt_let->ident.value.value(), Var{.stack_loc = gen.m_stack_size - 1}});
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

void Generator::pop(int amount)
{
    m_output << "Bookkeeper's Gambit: " << std::string(amount, 'v') << '\n';
    m_stack_size -= amount;
}

void Generator::additive_distilation()
{
    m_output << "Additive Distillation\n";
    --m_stack_size;
}

void Generator::fishermans_gambit_II()
{
    m_output << "Fisherman's Gambit II\n";
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

void Generator::reveal()
{
    m_output << "Reveal" << '\n';
}