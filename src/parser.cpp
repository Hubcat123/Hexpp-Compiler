#include "parser.hpp"

#include "util.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    :m_tokens(tokens), m_allocator(1024 * 1024 * 4)
{ }

std::optional<NodeProg*> Parser::parse()
{
    return parse_prog();
}

std::optional<NodeFunc*> Parser::parse_func(std::vector<TokenType> valid_types)
{
    // Check if statement is print
    if (peek().has_value() && std::find(valid_types.cbegin(), valid_types.cend(), peek().value().type) != valid_types.cend() &&
        peek(1).has_value() && peek(1).value().type == TokenType::paren_open)
    {
        size_t line = peek().value().line;

        // Consume starting tokens
        TokenType func_type = consume().type;
        consume();

        std::vector<NodeExpr*> exprs{};

        // Parse expresion
        while (std::optional<NodeExpr*> node_expr = parse_expr())
        {
            exprs.push_back(node_expr.value());

            if (!peek().has_value() || peek().value().type != TokenType::comma)
            {
                break;
            }

            consume();
        }

        // Check for closing tokens
        try_consume(TokenType::paren_close, ')');

        NodeFunc* stmt_func = m_allocator.alloc<NodeFunc>();
        stmt_func->func_type = func_type;
        stmt_func->exprs = exprs;
        stmt_func->line = line;
        return stmt_func;
    }

    return {};
}

std::optional<NodeTerm*> Parser::parse_term()
{
    if (peek().has_value())
    {
        size_t line = peek().value().line;
        // Check if term is a num literal
        if (peek().value().type == TokenType::num_lit)
        {
            NodeTermNumLit* node_term_num_lit = m_allocator.alloc<NodeTermNumLit>();
            node_term_num_lit->num_lit = consume();
            node_term_num_lit->line = line;
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = node_term_num_lit;
            node_term->line = line;
            return node_term;
        }
        // Check if term is an identifier
        else if (peek().value().type == TokenType::ident)
        {
            if (peek(1).has_value() && peek(1).value().type == TokenType::eq)
            {
                Token ident = consume();
                consume();
                if (std::optional<NodeExpr*> expr = parse_expr())
                {
                    NodeTermAssign* term_assign = m_allocator.alloc<NodeTermAssign>();
                    term_assign->ident = ident;
                    term_assign->expr = expr.value();
                    term_assign->line = line;
                    NodeTerm* term = m_allocator.alloc<NodeTerm>();
                    term->var = term_assign;
                    term->line = line;
                    return term;
                }
                else
                {
                    compilation_error("Expected expression", peek(-1).has_value() ? peek(-1).value().line : 1);
                }
            }
            else
            {
                NodeTermIdent* node_term_ident = m_allocator.alloc<NodeTermIdent>();
                node_term_ident->ident = consume();
                node_term_ident->line = line;
                NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
                node_term->var = node_term_ident;
                node_term->line = line;
                return node_term;
            }
        }
        // Check if term is a parentheses enclosed expression
        else if (peek().value().type == TokenType::paren_open)
        {
            consume();
            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                try_consume(TokenType::paren_close, ')');
                NodeTermParen* node_term_paren = m_allocator.alloc<NodeTermParen>();
                node_term_paren->expr = expr.value();
                node_term_paren->line = line;
                NodeTerm* term = m_allocator.alloc<NodeTerm>();
                term->var = node_term_paren;
                term->line = line;
                return term;
            }
            else
            {
                compilation_error("Expected expression", peek(-1).has_value() ? peek(-1).value().line : 1);
            }
        }
        // Check if term is a function
        else if (std::optional<NodeFunc*> func = parse_func(
            { TokenType::pow, TokenType::vec, TokenType::self, TokenType::pos, TokenType::forward, TokenType::eye_pos, TokenType::block_raycast, TokenType::block_normal_raycast }))
        {
            NodeTermFunc* node_term_func = m_allocator.alloc<NodeTermFunc>();
            node_term_func->func = func.value();
            node_term_func->line = line;
            NodeTerm* term = m_allocator.alloc<NodeTerm>();
            term->var = node_term_func;
            term->line = line;
            return term;
        }
    }

    return {};
}

std::optional<NodeExpr*> Parser::parse_expr(int min_prec)
{
    size_t line;
    if (peek().has_value())
    {
        line = peek().value().line;
    }

    std::optional<NodeTerm*> term_lhs = parse_term();
    if (!term_lhs.has_value())
    {
        return {};
    }

    NodeExpr* lhs_expr = m_allocator.alloc<NodeExpr>();
    lhs_expr->var = term_lhs.value();
    lhs_expr->line = line;
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
            compilation_error("Expected expression", peek(-1).has_value() ? peek(-1).value().line : 1);
        }

        NodeExprBin* expr_bin = m_allocator.alloc<NodeExprBin>();
        expr_bin->op_type = op_type;
        expr_bin->lhs = lhs_expr;
        expr_bin->rhs = rhs_expr.value();
        expr_bin->line = line;

        lhs_expr = m_allocator.alloc<NodeExpr>();
        lhs_expr->var = expr_bin;
        lhs_expr->line = line;
    }

    return lhs_expr;
}

std::optional<NodeScope*> Parser::parse_scope()
{
    if (peek().has_value() && peek().value().type == TokenType::curly_open)
    {
        size_t line = consume().line;

        std::vector<NodeStmt*> stmts{};
        while (std::optional<NodeStmt*> stmt = parse_stmt())
        {
            stmts.push_back(stmt.value());
        }

        try_consume(TokenType::curly_close, '}');

        NodeScope* stmt_scope = m_allocator.alloc<NodeScope>();
        stmt_scope->stmts = stmts;
        stmt_scope->line = line;
        return stmt_scope;
    }

    return {};
}

std::optional<NodeStmt*> Parser::parse_stmt()
{
    if (peek().has_value())
    {
        size_t line = peek().value().line;

        // Check if statement is function
        if (std::optional<NodeFunc*> func = parse_func(
            { TokenType::print, TokenType::mine }))
        {
            try_consume(TokenType::semi, ';');

            NodeStmtFunc* node_stmt_func = m_allocator.alloc<NodeStmtFunc>();
            node_stmt_func->func = func.value();
            node_stmt_func->line = line;
            NodeStmt* node_stmt = m_allocator.alloc<NodeStmt>();
            node_stmt->var = node_stmt_func;
            node_stmt->line = line;
            return node_stmt;
        }
        // Check if expression
        else if (std::optional<NodeExpr*> expr = parse_expr())
        {
            try_consume(TokenType::semi, ';');

            NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
            stmt->var = expr.value();
            stmt->line = line;
            return stmt;
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
            stmt_let->line = line;
            consume();

            // Parse expression
            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                stmt_let->expr = expr.value();
            }
            else
            {
                compilation_error("Invalid expression", peek(-1).has_value() ? peek(-1).value().line : 1);
            }

            // Check for closing token
            try_consume(TokenType::semi, ';');

            NodeStmt* node_stmt = m_allocator.alloc<NodeStmt>();
            node_stmt->var = stmt_let;
            node_stmt->line = line;
            return node_stmt;
        }
        // Check if if
        else if (peek().value().type == TokenType::if_)
        {
            consume();
            try_consume(TokenType::paren_open, '(');

            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                try_consume(TokenType::paren_close, ')');
                if (std::optional<NodeStmt*> if_stmt = parse_stmt())
                {
                    NodeStmtIf* stmt_if = m_allocator.alloc<NodeStmtIf>();
                    stmt_if->expr = expr.value();
                    stmt_if->stmt = if_stmt.value();
                    stmt_if->else_stmt = nullptr;
                    stmt_if->line = line;

                    // Check for potential else
                    if (peek().has_value() && peek().value().type == TokenType::else_)
                    {
                        consume();

                        if (std::optional<NodeStmt*> else_stmt = parse_stmt())
                        {
                            stmt_if->else_stmt = else_stmt.value();
                        }
                        else
                        {
                            compilation_error("Expected statement", peek(-1).has_value() ? peek(-1).value().line : 1);
                        }
                    }

                    NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
                    stmt->var = stmt_if;
                    stmt->line = line;
                    return stmt;
                }
                else
                {
                    compilation_error("Expected statement", peek(-1).has_value() ? peek(-1).value().line : 1);
                }
            }
            else
            {
                compilation_error("Expected expression", peek(-1).has_value() ? peek(-1).value().line : 1);
            }
        }
        // Check if scope
        else if (std::optional<NodeScope*> scope = parse_scope())
        {
            NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
            stmt->var = scope.value();
            stmt->line = line;
            return stmt;
        }
    }

    return {};
}

std::optional<NodeProg*> Parser::parse_prog()
{
    NodeProg* prog = m_allocator.alloc<NodeProg>();
    prog->line = 1;

    // Keep looping looking for statements until all found
    while (peek().has_value())
    {
        if (std::optional<NodeStmt*> node_stmt = parse_stmt())
        {
            prog->stmts.push_back(node_stmt.value());
        }
        else
        {
            compilation_error("Invalid statement", peek(-1).has_value() ? peek(-1).value().line : 1);
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
        compilation_error(std::string("Expected '") + tokenChar + '\'', peek(-1).has_value() ? peek(-1).value().line : 1);
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