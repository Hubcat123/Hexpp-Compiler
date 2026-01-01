#pragma once

#include "tokenization.hpp"

#include <variant>

#include "arena.hpp"

struct NodeExpr;
struct NodeStmt;

struct NodeTermNumLit {
    Token num_lit;
};

struct NodeTermIdent {
    Token ident;
};

struct NodeTermParen {
    NodeExpr* expr;
};

struct NodeExprAdd {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeExprSub {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeExprMulti {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeExprDiv {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeExprBin {
    std::variant<NodeExprAdd*, NodeExprSub*, NodeExprMulti*, NodeExprDiv*> var;
};

struct NodeTerm {
    std::variant<NodeTermNumLit*, NodeTermIdent*, NodeTermParen*> var;
};

struct NodeExpr {
    std::variant<NodeTerm*, NodeExprBin*> var;
};

struct NodePrint {
    NodeExpr* expr;
};

struct NodeStmtPrint {
    NodeExpr* expr;
};

struct NodeStmtLet {
    Token ident;
    NodeExpr* expr;
};

struct NodeStmtScope {
    std::vector<NodeStmt*> stmts;
};

struct NodeStmt {
    std::variant<NodeStmtPrint*, NodeStmtLet*, NodeStmtScope*> var;
};

struct NodeProg {
    std::vector<NodeStmt*> stmts;
};

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);

    std::optional<NodeProg*> parse();
private:
    std::optional<NodeTerm*> parse_term();
    std::optional<NodeExpr*> parse_expr(int min_prec = 0);
    std::optional<NodeStmt*> parse_stmt();
    std::optional<NodeProg*> parse_prog();

    void try_consume(TokenType type, char tokenChar);

    std::optional<Token> peek(int ahead = 0) const;
    Token consume();
    void consume(int amount);

    template<typename T>
    bool parse_bin_op(TokenType op_type, TokenType parse_type, NodeExprBin*& expr_bin, NodeExpr* lhs_expr, NodeExpr* rhs_expr)
    {
        if (op_type == parse_type)
        {
            T* expr_op = m_allocator.alloc<T>();
            expr_op->lhs = lhs_expr;
            expr_op->rhs = rhs_expr;
            expr_bin->var = expr_op;

            return true;
        }

        return false;
    }

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
    ArenaAllocator m_allocator;
};