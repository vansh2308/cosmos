
#include "Types/literal.h"
#include "Types/token.h"

#include <map>
#include <type_traits>
#include <utility>
#include <string>

namespace cosmos::Types{
    namespace {
        auto token_type_string(const TokenType value) -> const std::string& {
            static const std::map<TokenType, std::string> lookup_table{
                {TokenType::LEFT_PAREN, "LEFT_PAREN" },
                {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
                {TokenType::LEFT_BRACE, "LEFT_BRACE"},
                {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
                {TokenType::COMMA, "COMMA"},
                {TokenType::COLON, "COLON"},
                {TokenType::DOT, "DOT"},
                {TokenType::QUESTION, "QUESTION"},
                {TokenType::SEMICOLON, "SEMICOLON"},
                {TokenType::SLASH, "SLASH"},
                {TokenType::STAR, "STAR"},
                {TokenType::BANG, "BANG"},
                {TokenType::BANG_EQUAL, "BANG_EQUAL"},
                {TokenType::EQUAL, "EQUAL"},
                {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
                {TokenType::GREATER, "GREATER"},
                {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
                {TokenType::LESS, "LESS"},
                {TokenType::LESS_EQUAL, "LESS_EQUAL"},
                {TokenType::MINUS, "MINUS"},
                {TokenType::MINUS_MINUS, "MINUS_MINUS"},
                {TokenType::PLUS, "PLUS"},
                {TokenType::PLUS_PLUS, "PLUS_PLUS"},
                {TokenType::IDENTIFIER, "IDENTIFIER"},
                {TokenType::STRING, "STRING"},
                {TokenType::NUMBER, "NUMBER"},
                {TokenType::AND, "AND"},
                {TokenType::GALAXY, "GALAXY"},
                {TokenType::BLACKHOLE, "BLACKHOLE"},
                {TokenType::VOID, "VOID"},
                {TokenType::MISSION, "MISSION"},
                {TokenType::NAVIGATE, "NAVIGATE"},
                {TokenType::PROBE, "PROBE"},
                {TokenType::NIL, "NIL"},
                {TokenType::OR, "OR"},
                {TokenType::FLARE, "FLARE"},
                {TokenType::TRANSMIT, "TRANSMIT"},
                {TokenType::SUPER, "SUPER"},
                {TokenType::THIS, "THIS"},
                {TokenType::COSMIC, "COSMIC"},
                {TokenType::ATOM, "ATOM"},
                {TokenType::ORBIT, "ORBIT"},
                {TokenType::CSM_EOF, "CSM_EOF"}
            };

            return lookup_table.find(value)->second;
        }
    } // namespace


    Token::Token(TokenType p_type, std::string p_lexeme, OptionalLiteral p_literal, int p_line) 
    : type(p_type), 
    lexeme(std::move(p_lexeme)),
    literal(std::move(p_literal)),
    line(p_line) {}

    Token::Token(TokenType p_type, const char* p_lexeme, OptionalLiteral p_literal, int p_line) 
    : type(p_type), 
    lexeme(p_lexeme),
    literal(std::move(p_literal)),
    line(p_line) {}

    Token::Token(TokenType p_type, const char* p_lexeme)
    : type(p_type), lexeme(p_lexeme) {}

    auto Token::to_string() const -> std::string {
        std::string result = std::to_string(line) + " " + token_type_string(type) + " " + lexeme;
        result += literal.has_value() ? get_literal_string(literal.value()) : "No Literal";
        return result;
    }

    auto Token::get_type() const -> TokenType { return this->type; }

    auto Token::get_type_string() const -> const std::string& { return token_type_string(this->type); }
    
    auto Token::get_line() const -> int { return this->line; }

    auto Token::get_lexeme() const -> const std::string& { return this->lexeme; }

    auto Token::get_optional_literal() const -> const OptionalLiteral& { return this->literal; }

} // namespace cosmos::Types
