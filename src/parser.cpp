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
        // Check if term is a num literal
        if (peek().value().type == TokenType::num_lit)
        {
            NodeTermNumLit* node_term_num_lit = m_allocator.alloc<NodeTermNumLit>();
            node_term_num_lit->num_lit = consume();
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = node_term_num_lit;
            return node_term;
        }
        // Check if term is an identifier
        else if (peek().value().type == TokenType::ident)
        {
            NodeTermIdent* node_term_ident = m_allocator.alloc<NodeTermIdent>();
            node_term_ident->ident = consume();
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = node_term_ident;
            return node_term;
        }
        // Check if term is a parentheses enclosed expression
        else if (peek().value().type == TokenType::paren_open)
        {
            consume();
            NodeTermParen* node_term_paren = m_allocator.alloc<NodeTermParen>();
            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                try_consume(TokenType::paren_close, ')');
                node_term_paren->expr = expr.value();
                NodeTerm* term = m_allocator.alloc<NodeTerm>();
                term->var = node_term_paren;
                return term;
            }
            else
            {
                compilation_error("Expected expression");
            }
        }
    }

    return {};
}

std::optional<NodeExpr*> Parser::parse_expr(int min_prec)
{
    std::optional<NodeTerm*> term_lhs = parse_term();
    if (!term_lhs.has_value())
    {
        return {};
    }

    NodeExpr* lhs_expr = m_allocator.alloc<NodeExpr>();
    lhs_expr->var = term_lhs.value();
    while (true)
    {
        // Break if no next token
        std::optional<Token> curr_tok = peek();
        if (!curr_tok.has_value())
        {
            break;
        }

        // Break if next token not an operator or has lower precedance
        std::optional<int> prec = Tokenizer::bin_prec(curr_tok.value().type);
        if (!prec.has_value() || prec.value() < min_prec)
        {
            break;
        }

        TokenType op_type = consume().type;

        int next_min_prec = prec.value() + 1;
        std::optional<NodeExpr*> rhs_expr = parse_expr(next_min_prec);
        if (!rhs_expr.has_value())
        {
            compilation_error("Expected expression");
        }

        NodeExprBin* expr_bin = m_allocator.alloc<NodeExprBin>();
        bool is_op = parse_bin_op<NodeExprAdd>(op_type, TokenType::plus, expr_bin, lhs_expr, rhs_expr.value());
        is_op = is_op || parse_bin_op<NodeExprSub>(op_type, TokenType::dash, expr_bin, lhs_expr, rhs_expr.value());
        is_op = is_op || parse_bin_op<NodeExprMulti>(op_type, TokenType::star, expr_bin, lhs_expr, rhs_expr.value());
        is_op = is_op || parse_bin_op<NodeExprDiv>(op_type, TokenType::slash_forward, expr_bin, lhs_expr, rhs_expr.value());
        if (!is_op)
        {
            compilation_error("Unknown binary operator");
        }

        lhs_expr = m_allocator.alloc<NodeExpr>();
        lhs_expr->var = expr_bin;
    }

    return lhs_expr;
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
        else if (peek().value().type == TokenType::curly_open)
        {
            consume();
            
            std::vector<NodeStmt*> stmts{};
            while (std::optional<NodeStmt*> stmt = parse_stmt())
            {
                stmts.push_back(stmt.value());
            }

            try_consume(TokenType::curly_close, '}');

            NodeStmtScope* stmt_scope = m_allocator.alloc<NodeStmtScope>();
            stmt_scope->stmts = stmts;
            NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
            stmt->var = stmt_scope;
            return stmt;
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