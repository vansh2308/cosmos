#ifndef TYPES_TOKEN_H
#define TYPES_TOKEN_H
#pragma once 

#include <string>

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
    

}

#endif // TYPES_TOKEN_H