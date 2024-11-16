
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
                { TokenType::LEFT_PAREN, "LEFT_PAREN" },
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
                {TokenType::LOX_EOF, "LOX_EOF"}
            };

            return lookup_table.find(value)->second;
        }
    } // namespace

} // namespace cosmos::Types
