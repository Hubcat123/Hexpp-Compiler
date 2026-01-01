#pragma once

#include "tokenization.hpp"

#include <variant>

#include "arena.hpp"

struct NodeExpr;

struct NodeTermNumLit {
    Token num_lit;
};

struct NodeTermIdent {
    Token ident;
};

struct NodeExprAdd {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeExprMulti {
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeExprBin {
    std::variant<NodeExprAdd*, NodeExprMulti*> var;
};

struct NodeTerm {
    std::variant<NodeTermNumLit*, NodeTermIdent*> var;
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

struct NodeStmt {
    std::variant<NodeStmtPrint*, NodeStmtLet*> var;
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

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
    ArenaAllocator m_allocator;
};