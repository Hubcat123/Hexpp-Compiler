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

struct NodeVarListSubscript : Node {
    Token ident;
    NodeExpr* expr;
};

struct NodeVarIdent : Node {
    Token ident;
};

struct NodeTermVar : Node {
    std::variant<NodeVarIdent*, NodeVarListSubscript*> var;
};

struct NodeDefinedFunc : Node {
    Token ident;
    std::vector<NodeExpr*> exprs;
};

struct NodeTermNumLit : Node {
    Token num_lit;
};

struct NodeTermListLit : Node {
    std::vector<NodeExpr*> exprs;
};

struct NodeTermIotaLit : Node {
    Token pattern_lit;
};

struct NodeTermCallFunc : Node {
    NodeDefinedFunc* func;
};

struct NodeTermUn : Node {
    TokenType_ op_type;
    NodeTerm* term;
};

struct NodeTermUnPost : Node {
    TokenType_ op_type;
    NodeTermVar* vari;
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
    std::variant<NodeTermUn*, NodeTermUnPost*, NodeTermNumLit*, NodeTermListLit*, NodeTermIotaLit*, NodeTermBoolLit*, NodeTermNullLit*, NodeTermVar*, NodeTermParen*, NodeTermCallFunc*> var;
};

struct NodeExprBin : Node {
    TokenType_ op_type;
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

struct NodeStmtCallFunction : Node {
    NodeDefinedFunc* func;
};

struct NodeStmtReturn : Node {
    std::optional<NodeExpr*> expr;
};

struct NodeStmt : Node {
    std::variant<NodeStmtCallFunction*, NodeStmtReturn*, NodeExpr*, NodeStmtLet*, NodeStmtIf*, NodeStmtWhile*, NodeScope*> var;
};

struct NodeGlobalLet : Node {
    Token ident;
    NodeExpr* expr;
};

struct NodeFunctionDefVoid : Node {
    Token ident;
    std::vector<Token> params;
    NodeScope* scope;
};

struct NodeFunctionDefRet : Node {
    Token ident;
    std::vector<Token> params;
    NodeScope* scope;
};

struct NodeFunctionDef : Node {
    std::variant<NodeFunctionDefVoid*, NodeFunctionDefRet*> var;
};

struct NodeProg : Node {
    std::vector<NodeGlobalLet*> vars;
    std::vector<NodeFunctionDef*> funcs;
    NodeFunctionDef* main_ = nullptr;
};

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);

    std::optional<NodeProg*> parse();
private:
    std::optional<NodeDefinedFunc*> parse_defined_func();
    std::optional<NodeTerm*> parse_term();
    std::optional<NodeExpr*> parse_expr(int min_prec = 0, NodeTerm* first_term = nullptr);
    std::optional<NodeScope*> parse_scope();
    std::optional<NodeStmt*> parse_stmt();
    std::optional<NodeFunctionDef*> parse_func_def();
    std::optional<NodeProg*> parse_prog();

    void try_consume(TokenType_ type, char tokenChar);

    std::optional<Token> peek(int ahead = 0) const;
    Token consume();
    void consume(int amount);

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
    ArenaAllocator m_allocator;
};