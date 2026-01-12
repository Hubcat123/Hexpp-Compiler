#pragma once

#include "tokenization.hpp"

#include <variant>

#include "arena.hpp"

struct Node {
    size_t line;
};

struct NodeExpr;
struct NodeStmt;
struct NodeTerm;

struct NodeFunc : Node {
    TokenType func_type;
    std::vector<NodeExpr*> exprs;
};

struct NodeTermNumLit : Node {
    Token num_lit;
};

struct NodeTermIdent : Node {
    Token ident;
};

struct NodeTermFunc : Node {
    bool isMemberFunc;
    NodeFunc* func;
};

struct NodeTermUn : Node {
    TokenType op_type;
    std::optional<Token> ident;
    NodeTerm* term;
};

struct NodeTermUnPost : Node {
    TokenType op_type;
    Token ident;
};

struct NodeTermParen : Node {
    NodeExpr* expr;
};

struct NodeTermBoolLit : Node {
    Token bool_;
};

struct NodeTermNullLit : Node {
};

struct NodeTerm : Node {
    std::variant<NodeTermUn*, NodeTermUnPost*, NodeTermNumLit*, NodeTermBoolLit*, NodeTermNullLit*, NodeTermIdent*, NodeTermParen*, NodeTermFunc*> var;
};

struct NodeExprBin : Node {
    TokenType op_type;
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeExpr : Node {
    std::variant<NodeTerm*, NodeExprBin*> var;
};

struct NodeStmtLet : Node {
    Token ident;
    NodeExpr* expr;
};

struct NodeScope : Node {
    std::vector<NodeStmt*> stmts;
};

struct NodeStmtIf : Node {
    NodeExpr* expr;
    NodeStmt* stmt;
    NodeStmt* else_stmt;
};

struct NodeStmtWhile : Node {
    NodeExpr* expr;
    NodeStmt* stmt;
};

struct NodeStmtFunc : Node {
    NodeFunc* func;
};

struct NodeStmt : Node {
    std::variant<NodeStmtFunc*, NodeExpr*, NodeStmtLet*, NodeStmtIf*, NodeStmtWhile*, NodeScope*> var;
};

struct NodeProg : Node {
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