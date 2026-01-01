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

struct NodeExprBin {
    TokenType op_type;
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeFunc {
    TokenType func_type;
    std::vector<NodeExpr*> exprs;
};

struct NodeTermFunc {
    NodeFunc* func;
};

struct NodeTerm {
    std::variant<NodeTermNumLit*, NodeTermIdent*, NodeTermParen*, NodeTermFunc*> var;
};

struct NodeExpr {
    std::variant<NodeTerm*, NodeExprBin*> var;
};

struct NodeStmtLet {
    Token ident;
    NodeExpr* expr;
};

struct NodeScope {
    std::vector<NodeStmt*> stmts;
};

struct NodeStmtIf {
    NodeExpr* expr;
    NodeStmt* stmt;
};

struct NodeStmtFunc {
    NodeFunc* func;
};

struct NodeStmt {
    std::variant<NodeStmtFunc*, NodeExpr*, NodeStmtLet*, NodeStmtIf*, NodeScope*> var;
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
    std::optional<NodeFunc*> parse_func(std::vector<TokenType> valid_types);
    std::optional<NodeTerm*> parse_term();
    std::optional<NodeExpr*> parse_expr(int min_prec = 0);
    std::optional<NodeScope*> parse_scope();
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