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

    void RDParser::cosume_semicolon_or_error() {
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

} // namespace cosmos::Parser

