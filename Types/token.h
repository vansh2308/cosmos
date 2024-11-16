#ifndef TYPES_TOKEN_H
#define TYPES_TOKEN_H
#pragma once 

#include <string>

#include "literal.h"

namespace cosmos::Types{
    enum class TokenType{
        // Single-character tokens.
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        COLON,
        COMMA,
        DOT,
        QUESTION,
        SEMICOLON,
        SLASH,
        STAR,       

        // One or two character tokens.
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,
        MINUS,
        MINUS_MINUS,
        PLUS,
        PLUS_PLUS,      

        // Literals.
        IDENTIFIER,
        STRING,
        NUMBER,     

        // Keywords.
        AND,
        GALAXY, //CLASS
        BLACKHOLE, //ELSE
        VOID, // LOX_FALSE
        MISSION, //FUNC
        NAVIGATE, //FOR 
        PROBE, //IF 
        NIL,
        OR,
        FLARE, //PRINT 
        TRANSMIT, //RETURN
        SUPER,
        THIS,
        COSMIC, // LOX_TRUE
        ATOM, //VAR
        ORBIT, //WHILE      
        LOX_EOF
    };


    class Token{
        public: 
        Token(TokenType p_type, std::string p_lexeme, OptionalLiteral p_literal, int p_line);
        Token(TokenType p_type, const char* p_lexeme, OptionalLiteral p_literal, int p_line);
        Token(TokenType p_type, const char* p_lexeme);

        [[nodiscard]] auto to_string() const -> std::string;
        [[nodiscard]] auto get_type() const -> TokenType;
        [[nodiscard]] auto get_type_string() const -> const std::string&;
        [[nodiscard]] auto get_line() const -> int;
        [[nodiscard]] auto get_lexeme() const -> const std::string&;
        [[nodiscard]] auto get_optional_literal() const -> const OptionalLiteral&; 

        private:
        const TokenType type;
        const std::string lexeme;
        OptionalLiteral literal = std::nullopt;
        const int line = -1;
    };
    
} // namespace cosmos::Types

#endif // TYPES_TOKEN_H