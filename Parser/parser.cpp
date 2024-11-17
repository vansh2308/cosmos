#include "parser.h"
// WIP: Debugger 

#include <exception>
#include <functional>
#include <initializer_list>
#include <vector>
#include <memory>
#include <optional>
#include <type_traits>
#include <variant>


namespace cosmos::Parser{
    using Types::Token;
    using Types::TokenType;

    RDParser::RDParser(const std::vector<Token>& p_tokens): tokens(p_tokens) {
        this->current_iter = this->tokens.begin();
    }
    
    void RDParser::advance(){
        if(is_at_end()) ++current_iter;
    }

    auto RDParser::consume_any_binary_exps(const std::initializer_list<TokenType>& types, ExprPtrVariant expr, const parser_fn& f) -> ExprPtrVariant{
        while(match(types)){
            Token op = get_token_and_advance();
            expr = AST::create_binary_EPV(std::move(expr), op, std::invoke(f, this));
        }
        return expr;
    }


    void RDParser::consume_or_error(TokenType tType, const std::string& errorMessage) {
        if (get_current_token_type() == tType) return advance();
        throw error(errorMessage + " Got: " + peek().to_string());
    }

    auto RDParser::consume_one_literal(const std::string& str) -> ExprPtrVariant {
        advance();
        return AST::create_literal_EPV(Types::make_optional_literal(str));
    }

    auto RDParser::consume_one_literal() -> ExprPtrVariant {
        return AST::create_literal_EPV(get_token_and_advance().get_optional_literal());
    }

    auto RDParser::consume_super() -> ExprPtrVariant {
        Token super = get_token_and_advance();
        consume_or_error(TokenType::DOT, "Expected a '.' after 'super' keyword");
        if (!match(TokenType::IDENTIFIER))
            throw error("Expected an identifier after 'super.' ");
        Token method = get_token_and_advance();
        return AST::create_super_EPV(std::move(super), std::move(method));
    }

    auto RDParser::consume_grouping_expr() -> ExprPtrVariant {
        advance();
        ExprPtrVariant expr = expression();
        consume_or_error(TokenType::RIGHT_PAREN, std::to_string(peek().get_line()) + " Expected a closing paren after expression.");
        return AST::create_grouping_EPV(std::move(expr));
    }

    auto RDParser::consume_postfix_expr(ExprPtrVariant expr) -> ExprPtrVariant {
        while (match({TokenType::PLUS_PLUS, TokenType::MINUS_MINUS})) {
            expr = AST::create_postfix_EPV(std::move(expr), get_token_and_advance());
        }
        return expr;
    }

    auto RDParser::consume_unary_expr() -> ExprPtrVariant {
        return AST::create_unary_EPV(get_token_and_advance(), unary());
    }

    auto RDParser::consume_var_expr() -> ExprPtrVariant {
        Token var_name = get_token_and_advance();
        return AST::create_variable_EPV(var_name);
    }

    void RDParser::consume_semicolon_or_error() {
        consume_or_error(TokenType::SEMICOLON, "Expected a ';'");
    }

    auto RDParser::error(const std::string& eMessage) -> RDParser::RDParserError {
        report_error(eMessage);
        return RDParser::RDParserError();
    }

    auto RDParser::get_current_token_type() const -> TokenType {
        return current_iter->get_type();
    }

    auto RDParser::get_token_and_advance() -> Token {
        Token token = peek();
        advance();
        return token;
    }

    auto RDParser::is_at_end() const -> bool {
        return peek().get_type() == TokenType::CSM_EOF;
    }

    auto RDParser::match(Types::TokenType type) const -> bool {
        if (is_at_end()) return false;
        return (type == get_current_token_type());
    }  

    auto RDParser::match(const std::initializer_list<Types::TokenType>& types) const -> bool {
        bool result = false;
        for (const auto& type : types) result = (result || match(type));
        return result;
    }

    auto RDParser::match_next(Types::TokenType type) -> bool {
        advance();
        bool result = match(type);
        --current_iter;
        return result;
    }

    auto RDParser::peek() const -> Token { return *current_iter; }

    void RDParser::report_error(const std::string& message) {
        const Token& token = peek();
        std::string error = message;
        if (token.get_type() == TokenType::CSM_EOF)
            error = " at end: " + error;
        else
            error = " at '" + token.get_lexeme() + "': " + error;

        // WIP: Add ereporter
        // eReporter.setError(token.getLine(), error);
    }

    void RDParser::synchronize() {
        while (!is_at_end()) {
            switch (get_current_token_type()) {
                case TokenType::SEMICOLON: advance(); return;
                case TokenType::GALAXY:
                case TokenType::MISSION:
                case TokenType::ATOM:
                case TokenType::NAVIGATE:
                case TokenType::PROBE:
                case TokenType::ORBIT:
                case TokenType::FLARE:
                case TokenType::TRANSMIT: return;
      default:
        // WIP: Add debugger
        // ErrorsAndDebug::debugPrint("Discarding extranuous token:" + peek().get_lexeme());
        advance();
    }
  }
}

    void RDParser::throw_on_error_production(
        const std::initializer_list<Types::TokenType>& types, const parser_fn& f) {
            if (match(types)) {
                auto errObj = error("Missing left hand operand");
                advance();
                ExprPtrVariant expr = std::invoke(f, this);  // We check the rest of the expression anyways to see if there are any errors there
            throw errObj;
        }
    }

    void RDParser::throw_on_error_productions() {
        throw_on_error_production({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}, &RDParser::equality);
        throw_on_error_production({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}, &RDParser::comparison);
        throw_on_error_production({TokenType::PLUS}, &RDParser::addition);
        throw_on_error_production({TokenType::STAR, TokenType::SLASH}, &RDParser::multiplication);
    }


    // ----------------- Grammar rules -----------------
    // ----------------- Statements --------------------

    // program → declaration* LOX_EOF
    void RDParser::program(){
        try {
            while(!is_at_end()){
                std::optional<StmtPtrVariant> opt_stmt = declaration();
                if(opt_stmt.has_value()) statements.push_back(std::move(opt_stmt.value()));
            }
        } catch (const std::exception& e) {
            std::string error_msg = "Caught unhandled exception: ";
            error_msg += e.what();
            // WIP: add ereporter
        }
    }

    // declaration → varDecl | funcDecl | classDecl | statement;
    auto RDParser::declaration() -> std::optional<StmtPtrVariant> {
        try{
            if(match(TokenType::ATOM)){
                advance();
                return varDecl();
            }
            if(match(TokenType::MISSION) && match_next(TokenType::IDENTIFIER)){
                advance();
                return funcDecl("function");
            }
            if(match(TokenType::GALAXY)){
                advance();
                return classDecl();
            }
            return statement();

        } catch(const RDParserError& e){
            // WIP: Add debugger 
            return std::nullopt;
        }
    }

    // varDecl     → "var" IDENTIFIER ("=" expression)? ";" ;
    auto RDParser::varDecl() -> StmtPtrVariant {
        if(match(TokenType::IDENTIFIER)){
            Token var_name = get_token_and_advance();
            std::optional<ExprPtrVariant> initializer = std::nullopt;
            if(match(TokenType::EQUAL)){
                advance();
                initializer = expression();
            }
            consume_semicolon_or_error();
            return AST::create_var_SPV(var_name, std::move(initializer));
        }

        throw error("Expected variable name after the atom keyword");
    }


    // parameters → IDENTIFIER ( "," IDENTIFIER )* ;
    auto RDParser::parameters() -> std::vector<Token>{
        std::vector<Token> params;
        do {
            if(peek().get_type() != TokenType::IDENTIFIER){
                throw error("Expected an identifier for parameter.");
            }
            params.emplace_back(get_token_and_advance());
        } while ([this]() -> bool{
            if(match(TokenType::COMMA)){
                advance();
                return true;
            }
            return false;
        }());
        return params;
    }


    // funcBody     → "(" parameters? ")" "{" declaration "}";
    auto RDParser::funcBody(const std::string& kind) -> ExprPtrVariant {
        consume_or_error(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " decl.");
        std::vector<Token> params = match(TokenType::RIGHT_PAREN) ? std::vector<Token>() : parameters();
        consume_or_error(TokenType::RIGHT_PAREN, "Expect ')' after " + kind + " params.");
        consume_or_error(TokenType::LEFT_BRACE, "Expect '{' after " + kind + " params.");

        std::vector<StmtPtrVariant> fn_body;
        while(!match(TokenType::RIGHT_BRACE) && !is_at_end()){
            if(auto opt_stmt = declaration(); opt_stmt.has_value()){
                fn_body.push_back(std::move(opt_stmt.value()));
            }
        }

        consume_or_error(TokenType::RIGHT_BRACE, "Expect '}' after " + kind + " body.");
        return AST::create_func_EPV(std::move(params), std::move(fn_body));
    }


    // funcDecl    → IDENTIFIER funcBody;
    auto RDParser::funcDecl(const std::string& kind) -> StmtPtrVariant{
        if(match(TokenType::IDENTIFIER)){
            Token fn_name = get_token_and_advance();
            return AST::create_func_SPV(fn_name, std::get<AST::FuncExprPtr>(funcBody(kind)));
        }
        throw error("Expected a " + kind + " name after the mission keyword");
    }


    // classDecl   → "class" IDENTIFIER ("<" IDENTIFIER)? "{" funcDecl* "}" ;
    auto RDParser::classDecl() -> StmtPtrVariant{
        if(match(TokenType::IDENTIFIER)){
            Token cls_name = get_token_and_advance();

            std::optional<ExprPtrVariant> super_class = [&]() -> std::optional<ExprPtrVariant>{
                if(match(TokenType::LESS)){
                    advance();
                    if(match(TokenType::IDENTIFIER)){
                        auto super_class_var = consume_var_expr();
                        if((std::get<AST::VariableExprPtr>(super_class_var)->varName.get_lexeme() == cls_name.get_lexeme())){
                            throw error("A class can't inherit from itself");
                        }
                        return super_class_var;
                    }
                    throw error("Expected a superclass name after '<'");
                }
                return std::nullopt;
            }();

            consume_or_error(TokenType::LEFT_BRACE, "Expected '{' after class name. ");
            std::vector<StmtPtrVariant> methods;
            while(!match(TokenType::RIGHT_BRACE) && !is_at_end()){
                methods.push_back(funcDecl("method"));
            }
            consume_or_error(TokenType::RIGHT_BRACE, "Expected '}' after class name. ");
            return AST::create_class_SPV(std::move(cls_name), std::move(super_class), std::move(methods));
        }
        throw error("Expected a name after the mission keyword");
    }



    // statement   → exprStmt | printStmt | blockStmt | ifStmt | whileStmt |
    // statement   → forStmt;
    auto RDParser::statement() -> StmtPtrVariant{
        if(match(TokenType::FLARE)) return printStmt();
        if(match(TokenType::LEFT_BRACE)) return blockStmt();
        if(match(TokenType::PROBE)) return ifStmt();
        if(match(TokenType::ORBIT)) return whileStmt();
        if(match(TokenType::NAVIGATE)) return forStmt();
        if(match(TokenType::TRANSMIT)) return returnStmt();
        return exprStmt();
    }

    // exprStmt    → expression ';' ;
    auto RDParser::exprStmt() -> StmtPtrVariant{
        auto expr = expression();
        consume_semicolon_or_error();
        return AST::create_expr_SPV(std::move(expr));
    }


    // printStmt   → "print" expression ';' ;
    auto RDParser::printStmt() -> StmtPtrVariant{
        advance();
        ExprPtrVariant print_expr = expression();
        consume_semicolon_or_error();
        return AST::create_print_SPV(std::move(print_expr));
    }


    // blockStmt   → "{" declaration "}"
    auto RDParser::blockStmt() -> StmtPtrVariant{
        advance(); // consume {
        
        std::vector<StmtPtrVariant> statements;
        while(!match(TokenType::RIGHT_BRACE) && !is_at_end()){
            auto opt_stmt = declaration();
            if(opt_stmt.has_value()) statements.push_back(std::move(opt_stmt.value()));
        }

        consume_or_error(TokenType::RIGHT_BRACE, "Expected '}' after block");
        return AST::create_block_SPV(std::move(statements));
    }


    // ifStmt      → "if" "(" expression ")" statement ("else" statement)? ;
    auto RDParser::ifStmt() -> StmtPtrVariant{
        advance();
        consume_or_error(TokenType::LEFT_PAREN, "Expecte '(' after probe.");
        ExprPtrVariant condition = expression();
        consume_or_error(TokenType::RIGHT_PAREN, "Expecte ')' after probe condition.");

        StmtPtrVariant then_branch = statement();
        std::optional<StmtPtrVariant> else_branch = std::nullopt;
        if(match(TokenType::BLACKHOLE)){
            advance();
            else_branch = std::make_optional(statement());
        }


        return AST::create_if_SPV(std::move(condition), std::move(then_branch), std::move(else_branch));
    }


    // whileStmt   → "while" "(" expression ")" statement;
    auto RDParser::whileStmt() -> StmtPtrVariant {
        advance();
        consume_or_error(TokenType::LEFT_PAREN, "Expecte '(' after orbit.");
        ExprPtrVariant condition = expression();
        consume_or_error(TokenType::RIGHT_PAREN, "Expecte ')' after orbit condition.");

        return AST::create_while_SPV(std::move(condition), statement());
    }


    // forStmt     → "for" "("
    //                          (varDecl | exprStmnt | ";")
    //                          expression? ";"
    //                          expression?
    //                     ")"
    //                statement;
    auto RDParser::forStmt() -> StmtPtrVariant {
        advance();
        consume_or_error(TokenType::LEFT_PAREN, "Expected '(' after naviagate.");

        std::optional<StmtPtrVariant> initializer = std::nullopt;
        if(match(TokenType::SEMICOLON)){
            advance();
        } else if (match(TokenType::ATOM)){
            advance();
            initializer = std::make_optional(varDecl());
        } else {
            initializer = std::make_optional(exprStmt());
        }

        std::optional<ExprPtrVariant> condition = std::nullopt;
        if(!match(TokenType::SEMICOLON)){
            condition = std::make_optional(expression());
        }
        consume_semicolon_or_error();

        std::optional<ExprPtrVariant> increment = std::nullopt;
        if(!match(TokenType::RIGHT_PAREN)){
            increment = std::make_optional(expression());
        }
        consume_or_error(TokenType::RIGHT_PAREN, "Expecte ')' after 'for' clauses.");

        StmtPtrVariant loop_body = statement();
        return AST::create_for_SPV(std::move(initializer), std::move(condition), std::move(increment), std::move(loop_body));
    }


    // returnStmt  → "return" (expression)? ";";
    auto RDParser::returnStmt() -> StmtPtrVariant{
        Token ret = get_token_and_advance();
        std::optional<AST::ExprPtrVariant> value = std::nullopt;
        if(!match(TokenType::SEMICOLON)){
            value = expression();
        }

        consume_semicolon_or_error();
        return AST::create_ret_SPV(std::move(ret), std::move(value));
    }


    // ----------------- Expressions --------------------

    // expression → comma;
    auto RDParser::expression() -> ExprPtrVariant { return comma(); }

    // comma → assignment ("," assignment)*;
    auto RDParser::comma() -> ExprPtrVariant{
        return consume_any_binary_exps({TokenType::COMMA}, assignment(), &RDParser::assignment);
    }

    // assignment  → (call ".")? IDENTIFIER "=" assignment | condititional;
    auto RDParser::assignment() -> ExprPtrVariant{
        ExprPtrVariant expr = conditional();

        if(match(TokenType::EQUAL)){
            advance();
            if(std::holds_alternative<AST::VariableExprPtr>(expr)){
                Token var_name = std::get<AST::VariableExprPtr>(expr)->varName;
                return AST::create_assignment_EPV(var_name, assignment());
            }
            if(std::holds_alternative<AST::GetExprPtr>(expr)){
                auto& get_expr = std::get<AST::GetExprPtr>(expr);
                return AST::create_set_EPV(std::move(get_expr->expr), std::move(get_expr->name), assignment());
            }

            throw error("Invalid assignment target.");
        }

        return expr;
    }


    // conditional → logical_or ("?" expression ":" conditional)?;
    // conditional → equality ("?" expression ":" conditional)?
    auto RDParser::conditional() -> ExprPtrVariant{
        ExprPtrVariant expr = logical_or();
        if(match(TokenType::QUESTION)){
            Token op = get_token_and_advance();
            ExprPtrVariant then_branch = expression();
            consume_or_error(TokenType::COLON, "Expected a colon after ternary operator");
            return AST::create_conditional_EPV(std::move(expr), std::move(then_branch), conditional());
        }

        return expr;
    }


    // logical_or  → logical_and ("or" logical_and)*;
    auto RDParser::logical_or() -> ExprPtrVariant {
        ExprPtrVariant expr = logical_and();
        while(match(TokenType::OR)){
            Token op = get_token_and_advance();
            expr = AST::create_logical_EPV(std::move(expr), op, logical_and());
        }
        return expr;
    }


    // logical_and → equality ("and" equality)*;
    auto RDParser::logical_and() -> ExprPtrVariant {
        ExprPtrVariant expr = equality();
        while(match(TokenType::AND)){
            Token op = get_token_and_advance();
            expr = AST::create_logical_EPV(std::move(expr), op, equality());
        }

        return expr;
    }


    // equality   → comparison(("!=" | "==") comparison) *;
    auto RDParser::equality() -> ExprPtrVariant {
        auto equality_types = {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL};
        return consume_any_binary_exps(equality_types, comparison(), &RDParser::comparison);
    }

    // comparison → addition((">" | ">=" | "<" | "<=") addition) *;
    auto RDParser::comparison() -> ExprPtrVariant {
        auto comparator_types = { TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL };
        return consume_any_binary_exps(comparator_types, addition(), &RDParser::addition);
    }

    // addition   → multiplication(("-" | "+") multiplication) *;
    auto RDParser::addition() -> ExprPtrVariant {
        auto addition_types = {TokenType::PLUS, TokenType::MINUS};
        return consume_any_binary_exps(addition_types, multiplication(), &RDParser::multiplication);
    }

    // multi...   → unary(("/" | "*") unary) *;
    auto RDParser::multiplication() -> ExprPtrVariant {
        auto mult_types = {TokenType::SLASH, TokenType::STAR};
        return consume_any_binary_exps(mult_types, unary(), &RDParser::unary);
    }

    // unary      → ("!" | "-" | "--" | "++") unary | postfix;
    auto RDParser::unary() -> ExprPtrVariant {
        auto unary_types = {TokenType::BANG, TokenType::MINUS, TokenType::PLUS_PLUS, TokenType::MINUS_MINUS};
        if(match(unary_types)) return consume_unary_expr();
        return postfix();
    }

    // postfix    → primary ("++" | "--")*;
    auto RDParser::postfix() -> ExprPtrVariant {
        return consume_postfix_expr(call());
    }

    // call        → primary ( "(" arguments? ")" | "." IDENTIFIER )*;
    auto RDParser::call() -> ExprPtrVariant {
        auto expr = expression();
        while(true){
            if(match(TokenType::LEFT_PAREN)){
                advance();
                std::vector<ExprPtrVariant> args;
                if(!match(TokenType::RIGHT_PAREN)) args = arguments();
                if(!match(TokenType::RIGHT_PAREN)) throw error("Expected ')' after function invocation.");

                expr = AST::create_call_EPV(std::move(expr), get_token_and_advance(), std::move(args));
            } else if (match(TokenType::DOT)){
                advance();
                Token name = match(TokenType::IDENTIFIER) ? get_token_and_advance() : throw error("Expected a name after '.'.");
                expr = AST::create_get_EPV(std::move(expr), std::move(name));
            } else {
                break;
            }
        }

        return expr;
    }



    // arguments   → assignment  ( "," assignment )* ;
    auto RDParser::arguments() -> std::vector<ExprPtrVariant> {
        std::vector<ExprPtrVariant> args;
        args.push_back(assignment());

        while(match(TokenType::COMMA)){
            advance();
            if(args.size() >= MAX_ARGS){
                throw error("A function can't be invoked with more than 255 arguments");
            }

            args.push_back(assignment());
        }
        return args;
    }


    // primary    → NUMBER | STRING | "false" | "true" | "nil";
    // primary    →  "(" expression ")" | IDENTIFIER;
    // primary     → "fun" funBody;
    // primary     → "super" "." IDENTIFIER;
    //  Error Productions:
    // primary    → ("!=" | "==") equality
    // primary    → (">" | ">=" | "<" | "<=") comparison
    // primary    → ("+")addition
    // primary → ("/" | "*") multiplication;
    auto RDParser::primary() -> ExprPtrVariant {
        if (match(TokenType::VOID)) return consume_one_literal("false");
        if (match(TokenType::COSMIC)) return consume_one_literal("true");
        if (match(TokenType::NIL)) return consume_one_literal("nil");
        if (match(TokenType::NUMBER)) return consume_one_literal();
        if (match(TokenType::STRING)) return consume_one_literal();
        if (match(TokenType::LEFT_PAREN)) return consume_grouping_expr();
        if (match(TokenType::THIS)) return AST::create_this_EPV(get_token_and_advance());
        if (match(TokenType::IDENTIFIER)) return consume_var_expr();
        if (match(TokenType::MISSION)) return funcBody("Anon-Function");
        if (match(TokenType::SUPER)) return consume_super();

        throw_on_error_productions();
        throw error("Expected an expression; Got something else.");
    }







    auto RDParser::parse() -> std::vector<StmtPtrVariant> {
        program();
        return std::move(this->statements);
    }


} // namespace cosmos::Parser

