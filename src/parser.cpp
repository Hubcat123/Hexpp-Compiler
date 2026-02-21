#include "parser.hpp"

#include "util.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    :m_tokens(tokens), m_allocator(1024 * 1024 * 4)
{ }

std::optional<NodeProg*> Parser::parse()
{
    return parse_prog();
}

std::optional<NodeDefinedFunc*> Parser::parse_defined_func()
{
    // Check if function is valid type
    if (peek().has_value() && peek().value().type == TokenType_::ident &&
        peek(1).has_value() && peek(1).value().type == TokenType_::paren_open)
    {
        size_t line = peek().value().line;

        // Consume starting tokens
        NodeDefinedFunc* def_func = m_allocator.alloc<NodeDefinedFunc>();
        def_func->ident = consume();
        def_func->exprs = std::vector<NodeExpr*>();
        def_func->line = line;
        consume();

        // Parse expresions
        while (std::optional<NodeExpr*> node_expr = parse_expr())
        {
            def_func->exprs.push_back(node_expr.value());

            if (!peek().has_value() || peek().value().type != TokenType_::comma)
            {
                break;
            }

            consume();
        }

        // Check for closing tokens
        try_consume(TokenType_::paren_close, ')');

        return def_func;
    }

    return {};
}

std::optional<NodeTerm*> Parser::parse_term()
{
    if (peek().has_value())
    {
        size_t line = peek().value().line;
        
        const std::vector<TokenType_> unaryOperandTypes = { TokenType_::dash, TokenType_::not_, TokenType_::double_dash, TokenType_::double_plus, TokenType_::tilde };
        // Check if term is pre unary operator
        if (std::find(unaryOperandTypes.cbegin(), unaryOperandTypes.cend(), peek().value().type) != unaryOperandTypes.end())
        {
            TokenType_ op_type = consume().type;

            if (std::optional<NodeTerm*> un_term = parse_term())
            {
                NodeTermUn* term_un = m_allocator.alloc<NodeTermUn>();
                term_un->op_type = op_type;
                term_un->term = un_term.value();
                term_un->line = line;
                NodeTerm* term = m_allocator.alloc<NodeTerm>();
                term->var = term_un;
                term->line = line;
                return term;
            }
            else
            {
                compilation_error("Expected term", peek(-1).value().line);
            }
        }
        // Check if term is a num literal
        else if (peek().value().type == TokenType_::num_lit)
        {
            NodeTermNumLit* node_term_num_lit = m_allocator.alloc<NodeTermNumLit>();
            node_term_num_lit->num_lit = consume();
            node_term_num_lit->line = line;
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = node_term_num_lit;
            node_term->line = line;
            return node_term;
        }
        // Check if term is a list lit
        else if (peek().value().type == TokenType_::square_open)
        {
            consume();

            NodeTermListLit* list_lit = m_allocator.alloc<NodeTermListLit>();
            list_lit->exprs = std::vector<NodeExpr*>();
            list_lit->line = line;

            // Loop capturing exprs
            while (std::optional<NodeExpr*> expr = parse_expr())
            {
                list_lit->exprs.push_back(expr.value());

                if (!peek().has_value() || peek().value().type != TokenType_::comma)
                {
                    break;
                }

                consume();
            }

            try_consume(TokenType_::square_close, ']');

            NodeTerm* term = m_allocator.alloc<NodeTerm>();
            term->var = list_lit;
            term->line = line;
            return term;
        }
        // Check if term is pattern lit
        else if (peek().value().type == TokenType_::pattern_lit)
        {
            NodeTermIotaLit* pattern_lit = m_allocator.alloc<NodeTermIotaLit>();
            pattern_lit->pattern_lit = consume();
            pattern_lit->line = line;
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = pattern_lit;
            node_term->line = line;
            return node_term;
        }
        // Check if term is bool lit
        else if (peek().value().type == TokenType_::bool_lit)
        {
            NodeTermBoolLit* bool_lit = m_allocator.alloc<NodeTermBoolLit>();
            bool_lit->bool_ = consume();
            bool_lit->line = line;
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = bool_lit;
            node_term->line = line;
            return node_term;
        }
        // Check if term is null lit
        else if (peek().value().type == TokenType_::null_lit)
        {
            consume();
            NodeTermNullLit* null_lit = m_allocator.alloc<NodeTermNullLit>();
            null_lit->line = line;
            NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
            node_term->var = null_lit;
            node_term->line = line;
            return node_term;
        }
        // Check if term is a function
        else if (std::optional<NodeDefinedFunc*> func = parse_defined_func())
        {
            NodeTermCallFunc* call_func = m_allocator.alloc<NodeTermCallFunc>();
            call_func->func = func.value();
            call_func->line = line;
            NodeTerm* term = m_allocator.alloc<NodeTerm>();
            term->var = call_func;
            term->line = line;
            return term;
        }
        // Check if term is an identifier
        else if (peek().value().type == TokenType_::ident)
        {
            // Check if is subscript var
            if (peek(1).has_value() && peek(1).value().type == TokenType_::square_open)
            {
                NodeVarListSubscript* var_list = m_allocator.alloc<NodeVarListSubscript>();
                var_list->ident = consume();
                var_list->line = line;

                // Consume subscript
                consume();
                std::optional<NodeExpr*> expr = parse_expr();
                if (!expr.has_value())
                {
                    compilation_error("Expected expression", line);
                }
                var_list->expr = expr.value();
                try_consume(TokenType_::square_close, ']');

                NodeTermVar* term_var = m_allocator.alloc<NodeTermVar>();
                term_var->var = var_list;
                term_var->line = line;

                // Check for post-op
                if (peek(1).has_value())
                {
                    const std::vector<TokenType_> postUnaryOperandTypes = { TokenType_::double_plus, TokenType_::double_dash };
                    // Check if term is post unary operator
                    if (std::find(postUnaryOperandTypes.cbegin(), postUnaryOperandTypes.cend(), peek(1).value().type) != postUnaryOperandTypes.end())
                    {
                        NodeTermUnPost* term_un_post = m_allocator.alloc<NodeTermUnPost>();
                        term_un_post->vari = term_var;
                        term_un_post->op_type = consume().type;
                        term_un_post->line = line;
                        NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
                        node_term->var = term_un_post;
                        node_term->line = line;
                        return node_term;
                    }
                }

                NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
                node_term->var = term_var;
                node_term->line = line;
                return node_term;
            }
            else
            {
                NodeVarIdent* var_ident = m_allocator.alloc<NodeVarIdent>();
                var_ident->ident = consume();
                var_ident->line = line;
                NodeTermVar* term_var = m_allocator.alloc<NodeTermVar>();
                term_var->var = var_ident;
                term_var->line = line;

                // Check for post-op
                if (peek().has_value())
                {
                    const std::vector<TokenType_> postUnaryOperandTypes = { TokenType_::double_plus, TokenType_::double_dash };
                    // Check if term is post unary operator
                    if (std::find(postUnaryOperandTypes.cbegin(), postUnaryOperandTypes.cend(), peek().value().type) != postUnaryOperandTypes.end())
                    {
                        NodeTermUnPost* term_un_post = m_allocator.alloc<NodeTermUnPost>();
                        term_un_post->vari = term_var;
                        term_un_post->op_type = consume().type;
                        term_un_post->line = line;
                        NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
                        node_term->var = term_un_post;
                        node_term->line = line;
                        return node_term;
                    }
                }

                NodeTerm* node_term = m_allocator.alloc<NodeTerm>();
                node_term->var = term_var;
                node_term->line = line;
                return node_term;
            }
        }
        // Check if term is a parentheses enclosed expression
        else if (peek().value().type == TokenType_::paren_open)
        {
            consume();
            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                try_consume(TokenType_::paren_close, ')');
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
    }

    return {};
}

std::optional<NodeExpr*> Parser::parse_expr(int min_prec, NodeTerm* first_term)
{
    size_t line;
    if (peek().has_value())
    {
        line = peek().value().line;
    }

    std::optional<NodeTerm*> term_lhs;

    // Check if we were passed an initial term
    if (first_term == nullptr)
    {
        term_lhs = parse_term();
        if (!term_lhs.has_value())
        {
            return {};
        }
    }
    else
    {
        term_lhs = first_term;
    }

    NodeExpr* lhs_expr = m_allocator.alloc<NodeExpr>();
    lhs_expr->var = term_lhs.value();
    lhs_expr->line = line;
    while (true)
    {
        // Make sure token has a value
        if (!peek().has_value())
        {
            break;
        }

        TokenType_ op_type = peek().value().type;

        int prec;

        // Make sure token is bin operator
        if (std::optional<int> op_prec = Tokenizer::bin_prec(op_type))
        {
            prec = op_prec.value();
        }
        else
        {
            break;
        }

        // Make sure operator has required precedence
        if (prec < min_prec)
        {
            break;
        }

        // Consume operator
        consume();

        int next_min_prec = prec + 1;
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
    if (peek().has_value() && peek().value().type == TokenType_::curly_open)
    {
        size_t line = consume().line;

        std::vector<NodeStmt*> stmts{};
        while (std::optional<NodeStmt*> stmt = parse_stmt())
        {
            stmts.push_back(stmt.value());
        }

        try_consume(TokenType_::curly_close, '}');

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

        // Check if statement is a function call
        if (std::optional<NodeDefinedFunc*> defined_func = parse_defined_func())
        {
            // If has a semi, make it a stmt call
            if (peek().has_value() && peek().value().type == TokenType_::semi)
            {
                consume();

                NodeStmtCallFunction* call_func = m_allocator.alloc<NodeStmtCallFunction>();
                call_func->func = defined_func.value();
                call_func->line = line;
                NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
                stmt->var = call_func;
                stmt->line = line;
                return stmt;
            }
            // If no semi, make it an expr call
            else
            {
                NodeTermCallFunc* call_func = m_allocator.alloc<NodeTermCallFunc>();
                call_func->func = defined_func.value();
                call_func->line = line;
                NodeTerm* term = m_allocator.alloc<NodeTerm>();
                term->var = call_func;
                term->line = line;

                if (std::optional<NodeExpr*> expr = parse_expr(0, term))
                {
                    try_consume(TokenType_::semi, ';');

                    NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
                    stmt->var = expr.value();
                    stmt->line = line;
                    return stmt;
                }
                else
                {
                    // Should be unreachable
                    compilation_error("Compiler failure: An error occured in the compiler due to failure to parse an expression. Please report bug. Problem found", line);
                }
            }
        }
        // Check if return
        else if (peek().value().type == TokenType_::return_)
        {
            consume();

            NodeStmtReturn* stmt_ret = m_allocator.alloc<NodeStmtReturn>();
            stmt_ret->expr = parse_expr();
            stmt_ret->line = line;

            try_consume(TokenType_::semi, ';');

            NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
            stmt->var = stmt_ret;
            stmt->line = line;
            return stmt;
        }
        // Check if expression
        else if (std::optional<NodeExpr*> expr = parse_expr())
        {
            try_consume(TokenType_::semi, ';');

            NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
            stmt->var = expr.value();
            stmt->line = line;
            return stmt;
        }
        // Check if var
        else if (
            peek().value().type == TokenType_::let && peek(1).has_value() &&
            peek(1).value().type == TokenType_::ident &&
            peek(2).has_value() && peek(2).value().type == TokenType_::eq)
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
            try_consume(TokenType_::semi, ';');

            NodeStmt* node_stmt = m_allocator.alloc<NodeStmt>();
            node_stmt->var = stmt_let;
            node_stmt->line = line;
            return node_stmt;
        }
        // Check if if
        else if (
            peek().value().type == TokenType_::if_ && peek(1).has_value() &&
            peek(1).value().type == TokenType_::paren_open)
        {
            consume(2);

            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                try_consume(TokenType_::paren_close, ')');
                if (std::optional<NodeStmt*> if_stmt = parse_stmt())
                {
                    NodeStmtIf* stmt_if = m_allocator.alloc<NodeStmtIf>();
                    stmt_if->expr = expr.value();
                    stmt_if->stmt = if_stmt.value();
                    stmt_if->else_stmt = nullptr;
                    stmt_if->line = line;

                    // Check for potential else
                    if (peek().has_value() && peek().value().type == TokenType_::else_)
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
        // Check if while
        else if (
            peek().value().type == TokenType_::while_ && peek(1).has_value() &&
            peek(1).value().type == TokenType_::paren_open)
        {
            consume(2);

            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                try_consume(TokenType_::paren_close, ')');

                if (std::optional<NodeStmt*> while_stmt = parse_stmt())
                {
                    NodeStmtWhile* stmt_while = m_allocator.alloc<NodeStmtWhile>();
                    stmt_while->expr = expr.value();
                    stmt_while->stmt = while_stmt.value();
                    stmt_while->line = line;
                    NodeStmt* stmt = m_allocator.alloc<NodeStmt>();
                    stmt->var = stmt_while;
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

std::optional<NodeFunctionDef*> Parser::parse_func_def()
{
    // Return if no more tokens
    if (!peek().has_value())
    {
        return {};
    }

    size_t line = peek().value().line;

    NodeFunctionDef* func_def = m_allocator.alloc<NodeFunctionDef>();
    func_def->line = line;

    // Check if void function
    if (peek().value().type == TokenType_::void_ && peek(1).has_value() &&
        peek(1).value().type == TokenType_::ident && peek(2).has_value() &&
        peek(2).value().type == TokenType_::paren_open)
    {
        consume();
        NodeFunctionDefVoid* func_void = m_allocator.alloc<NodeFunctionDefVoid>();
        func_void->ident = consume();
        func_void->line = line;
        consume();

        // Parse params
        while (peek().has_value() && peek().value().type == TokenType_::ident)
        {
            func_void->params.push_back(consume());

            if (!peek().has_value() || peek().value().type != TokenType_::comma)
            {
                break;
            }

            consume();
        }

        try_consume(TokenType_::paren_close, ')');

        // Parse scope of function
        if (std::optional<NodeScope*> scope = parse_scope())
        {
            func_void->scope = scope.value();
        }
        else
        {
            compilation_error("Expected scope", line);
        }

        func_def->var = func_void;
        return func_def;
    }
    // Check if ret function
    else if (
        peek().value().type == TokenType_::ret && peek(1).has_value() &&
        peek(1).value().type == TokenType_::ident && peek(2).has_value() &&
        peek(2).value().type == TokenType_::paren_open)
    {
        consume();
        NodeFunctionDefRet* func_void = m_allocator.alloc<NodeFunctionDefRet>();
        func_void->ident = consume();
        func_void->line = line;
        consume();

        // Parse params
        while (peek().has_value() && peek().value().type == TokenType_::ident)
        {
            func_void->params.push_back(consume());

            if (!peek().has_value() || peek().value().type != TokenType_::comma)
            {
                break;
            }

            consume();
        }

        try_consume(TokenType_::paren_close, ')');

        // Parse scope of function
        if (std::optional<NodeScope*> scope = parse_scope())
        {
            func_void->scope = scope.value();
        }
        else
        {
            compilation_error("Expected scope", line);
        }

        func_def->var = func_void;
        return func_def;
    }
    else
    {
        return {};
    }
}

std::optional<NodeProg*> Parser::parse_prog()
{
    NodeProg* prog = m_allocator.alloc<NodeProg>();
    prog->vars = std::vector<NodeGlobalLet*>();
    prog->funcs = std::vector<NodeFunctionDef*>();
    prog->line = 1;

    // Keep looping looking for statements until all found
    while (peek().has_value())
    {
        size_t line = peek().value().line;

        // Check if global var
        if (peek().value().type == TokenType_::let && peek(1).has_value() &&
            peek(1).value().type == TokenType_::ident &&
            peek(2).has_value() && peek(2).value().type == TokenType_::eq)
        {
            // Consume sarting tokens and grab ident
            consume();
            NodeGlobalLet* global_let = m_allocator.alloc<NodeGlobalLet>();
            global_let->ident = consume();
            global_let->line = line;
            consume();

            // Parse expression
            if (std::optional<NodeExpr*> expr = parse_expr())
            {
                global_let->expr = expr.value();
            }
            else
            {
                compilation_error("Expected expression", peek(-1).has_value() ? peek(-1).value().line : 1);
            }

            // Check for closing token
            try_consume(TokenType_::semi, ';');

            prog->vars.push_back(global_let);
        }
        // Check if function def
        else if (std::optional<NodeFunctionDef*> func_def = parse_func_def())
        {
            // Check if main function
            bool isMain = false;
            if (std::holds_alternative<NodeFunctionDefVoid*>(func_def.value()->var))
            {
                NodeFunctionDefVoid* func_void = std::get<NodeFunctionDefVoid*>(func_def.value()->var);
                if (func_void->ident.value == "main")
                {
                    // Make sure main function isn't being passed arguments
                    if (func_void->params.size() > 0)
                    {
                        compilation_error("Main function must not require arguments", line);
                    }

                    // Make sure not defining multiple main functions
                    if (prog->main_ != nullptr)
                    {
                        compilation_error("Second main function defined", line);
                    }

                    isMain = true;
                    NodeFunctionDef* main_def = m_allocator.alloc<NodeFunctionDef>();
                    main_def->var = func_void;
                    main_def->line = line;
                    prog->main_ = main_def;
                }
            }

            if (!isMain)
            {
                prog->funcs.push_back(func_def.value());
            }
        }
        else
        {
            compilation_error("Expected global statement", line);
        }
    }

    // Make sure a main function was defined
    if (prog->main_ == nullptr)
    {
        compilation_error("Main function never defined", 0);
    }

    return prog;
}

void Parser::try_consume(TokenType_ type, char tokenChar)
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