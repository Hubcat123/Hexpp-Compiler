#include "parser.hpp"

#include "util.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    :m_tokens(tokens), m_allocator(1024 * 1024 * 4)
{ }

std::optional<NodeProg*> Parser::parse()
{
    return parse_prog();
}

std::optional<NodeTerm*> Parser::parse_term()
{
    if (peek().has_value())
    {
        // Check if expresion is a num literal
        if (peek().value().type == TokenType::num_lit)
        {
            NodeTermNumLit* node_term_num_lit = m_allocator.alloc<NodeTermNumLit>();
            node_term_num_lit->num_lit = consume();
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = node_term_num_lit;
            return node_term;
        }
        // Check if expresion is an identifier
        else if (peek().value().type == TokenType::ident)
        {
            NodeTermIdent* node_term_ident = m_allocator.alloc<NodeTermIdent>();
            node_term_ident->ident = consume();
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = node_term_ident;
            return node_term;
        }
    }

    return {};
}

std::optional<NodeExprBin*> Parser::parse_expr_bin()
{
    if (std::optional<NodeExpr*> lhs = parse_expr())
    {
        if (peek().has_value())
        {
            if (peek().value().type == TokenType::plus)
            {
                consume();
                if (std::optional<NodeExpr*> rhs = parse_expr())
                {
                    NodeExprAdd* node_expr_add = m_allocator.alloc<NodeExprAdd>();
                    node_expr_add->lhs = lhs.value();
                    node_expr_add->rhs = rhs.value();
                    NodeExprBin* node_expr_bin = m_allocator.alloc<NodeExprBin>();
                    node_expr_bin->var = node_expr_add;
                    return node_expr_bin;
                }
                else
                {
                    compilation_error("Expected expression");
                }
            }
            else if (peek().value().type == TokenType::star)
            {
                consume();
                if (std::optional<NodeExpr*> rhs = parse_expr())
                {
                    NodeExprMulti* node_expr_multi = m_allocator.alloc<NodeExprMulti>();
                    node_expr_multi->lhs = lhs.value();
                    node_expr_multi->rhs = rhs.value();
                    NodeExprBin* node_expr_bin = m_allocator.alloc<NodeExprBin>();
                    node_expr_bin->var = node_expr_multi;
                    return node_expr_bin;
                }
                else
                {
                    compilation_error("Expected expression");
                }
            }
        }
    }

    return {};
}

std::optional<NodeExpr*> Parser::parse_expr()
{
    if (std::optional<NodeTerm*> node_term = parse_term())
    {
        if (peek().has_value() && peek().value().type == TokenType::plus)
        {
            consume();
            if (std::optional<NodeExpr*> rhs = parse_expr())
            {
                NodeExprAdd* node_expr_add = m_allocator.alloc<NodeExprAdd>();
                NodeExpr* lhs_expr = m_allocator.alloc<NodeExpr>();
                lhs_expr->var = node_term.value();
                node_expr_add->lhs = lhs_expr;
                node_expr_add->rhs = rhs.value();
                NodeExprBin* node_expr_bin = m_allocator.alloc<NodeExprBin>();
                node_expr_bin->var = node_expr_add;
                NodeExpr* node_expr = m_allocator.alloc<NodeExpr>();
                node_expr->var = node_expr_bin;
                return node_expr;
            }
            else
            {
                compilation_error("Expected expression");
            }
        }
        else if (peek().has_value() && peek().value().type == TokenType::star)
        {
            consume();
            if (std::optional<NodeExpr*> rhs = parse_expr())
            {
                NodeExprMulti* node_expr_multi = m_allocator.alloc<NodeExprMulti>();
                NodeExpr* lhs_expr = m_allocator.alloc<NodeExpr>();
                lhs_expr->var = node_term.value();
                node_expr_multi->lhs = lhs_expr;
                node_expr_multi->rhs = rhs.value();
                NodeExprBin* node_expr_bin = m_allocator.alloc<NodeExprBin>();
                node_expr_bin->var = node_expr_multi;
                NodeExpr* node_expr = m_allocator.alloc<NodeExpr>();
                node_expr->var = node_expr_bin;
                return node_expr;
            }
            else
            {
                compilation_error("Expected expression");
            }
        }
        else
        {
            NodeExpr* node_expr = m_allocator.alloc<NodeExpr>();
            node_expr->var = node_term.value();
            return node_expr;
        }
    }
    else if (std::optional<NodeExprBin*> node_expr_bin = parse_expr_bin())
    {
        NodeExpr* node_expr = m_allocator.alloc<NodeExpr>();
        node_expr->var = node_expr_bin.value();
        return node_expr;
    }

    return {};
}

std::optional<NodeStmt*> Parser::parse_stmt()
{
    if (peek().has_value())
    {
        // Check if statement is print
        if (peek().value().type == TokenType::print &&
            peek(1).has_value() && peek(1).value().type == TokenType::paren_open)
        {
            // Consume starting tokens
            consume(2);

            NodeStmtPrint* stmt_print;

            // Parse expresion
            if (std::optional<NodeExpr*> node_expr = parse_expr())
            {
                stmt_print = m_allocator.alloc<NodeStmtPrint>();
                stmt_print->expr = node_expr.value();
            }
            else
            {
                compilation_error("Invalid expression");
            }

            // Check for closing tokens
            try_consume(TokenType::paren_close, ')');
            try_consume(TokenType::semi, ';');

            NodeStmt* node_stmt = m_allocator.alloc<NodeStmt>();
            node_stmt->var = stmt_print;
            return node_stmt;
        }
        // Check if num var
        else if (
            peek().value().type == TokenType::let && peek(1).has_value() &&
            peek(1).value().type == TokenType::ident &&
            peek(2).has_value() && peek(2).value().type == TokenType::eq)
        {
            // Consume sarting tokens and grab ident
            consume();
            NodeStmtLet* stmt_let = m_allocator.alloc<NodeStmtLet>();
            stmt_let->ident = consume();
            consume();

            // Parse expression
            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                stmt_let->expr = expr.value();
            }
            else
            {
                compilation_error("Invalid expression");
            }

            // Check for closing token
            try_consume(TokenType::semi, ';');

            NodeStmt* node_stmt = m_allocator.alloc<NodeStmt>();
            node_stmt->var = stmt_let;
            return node_stmt;
        }
    }

    return {};
}

std::optional<NodeProg*> Parser::parse_prog()
{
    NodeProg* prog = m_allocator.alloc<NodeProg>();

    // Keep looping looking for statements until all found
    while (peek().has_value())
    {
        if (std::optional<NodeStmt*> node_stmt = parse_stmt())
        {
            prog->stmts.push_back(node_stmt.value());
        }
        else
        {
            compilation_error("Invalid statement");
        }
    }

    return prog;
}

void Parser::try_consume(TokenType type, char tokenChar)
{
    if (peek().has_value() && peek().value().type == type)
    {
        consume();
    }
    else
    {
        compilation_error(std::string("Expected '") + tokenChar + '\'');
    }
}

std::optional<Token> Parser::peek(int ahead) const
{
    if (m_index + ahead >= m_tokens.size())
    {
        return {};
    }
    
    return m_tokens.at(m_index + ahead);
}

Token Parser::consume()
{
    return m_tokens.at(m_index++);
}

void Parser::consume(int amount)
{
    m_index += amount;
}