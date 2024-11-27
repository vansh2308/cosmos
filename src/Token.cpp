
#include "../include/Token.hpp"
#include <algorithm>
#include <iostream>
#include <map>

Token::Token(TokenType type , std::string lexeme, unsigned int line) : type{type}, lexeme{std::move(lexeme)}, line{line} {

}

Token::Token(Token&& token) noexcept : type{token.type}, lexeme{std::move(token.lexeme)}, line{line} {

}

Token::Token(const Token& token) : type{token.type}, lexeme{token.lexeme}, line{token.line} {

}

std::ostream& operator<<(std::ostream& os, const TokenType type) {
    using enum TokenType;
    /* clang-format off */ 
    static const std::map<TokenType, std::string> type_map{
            {LEFT_PAREN,        "LEFT_PAREN"},
            {RIGHT_PAREN,       "RIGHT_PAREN"},
            {LEFT_BRACE,        "LEFT_BRACE"},
            {RIGHT_BRACE,       "RIGHT_BRACE"},
            {COMMA,             "COMMA"},
            {DOT,               "DOT"},
            {MINUS,             "MINUS"},
            {PLUS,              "PLUS"},
            {SLASH,             "SLASH"},
            {SEMICOLON,         "SEMICOLON"},
            {STAR,              "STAR"},
            {LEFT_BRACKET,      "LEFT_BRACKET"},
            {RIGHT_BRACKET,     "RIGHT_BRACKET"},
            {EXCLAMATION,       "EXCLAMATION"},
            {EXCLAMATION_EQUAL, "EXCLAMATION_EQUAL"},
            {EQUAL,             "EQUAL"},
            {EQUAL_EQUAL,       "EQUAL_EQUAL"},
            {GREATER,           "GREATER"},
            {GREATER_EQUAL,     "GREATER_EQUAL"},
            {LESS,              "LESS"},
            {LESS_EQUAL,        "LESS_EQUAL"},
            {MINUS_MINUS,       "MINUS_MINUS"},
            {PLUS_PLUS,         "PLUS_PLUS"},
            {IDENTIFIER,        "IDENTIFIER"},
            {STRING,            "STRING"},
            {NUMBER,            "NUMBER"},
            {AND,               "AND"},
            {OR,                "OR"},
            {NOVA,              "NOVA"},
            {PROBE,             "PROBE"},
            {BLACKHOLE,         "BLACKHOLE"},
            {ELPROBE,           "ELPROBE"},
            {VOID,              "VOID"},
            {COSMIC,            "COSMIC"},
            {MISSION,           "MISSION"},
            {NAVIGATE,          "NAVIGATE"},
            {ORBIT,             "ORBIT"},
            {NIL,               "NIL"},
            {FLARE,             "FLARE"},
            {TRANSMIT,          "TRANSMIT"},
            {SUPERNOVA,         "SUPERNOVA"},
            {THIS,              "THIS"},
            {ATOM,              "ATOM"},
            {LAMBDA,            "LAMBDA"},
            {EJECT,             "EJECT"},
            {WARP,              "WARP"},
            {_EOF,              "EOF"},
    };
    /* clang-format on */
    os << type_map.at(type);
    return os;
}


std::ostream& operator<<(std::ostream& os, const Token& token){
    os << token.type << ' '<< token.lexeme;
    return os;
}