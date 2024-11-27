#ifndef LEXER_HPP
#define LEXER_HPP 

#include "Token.hpp"
#include <unordered_map>
#include <vector>

class Lexer{
    public:
    explicit Lexer(std::string source);
    std::vector<Token> scan_tokens();
    static const std::unordered_map<std::string, TokenType> keywords;

    private:
    const std::string source;
    std::vector<Token> tokens;
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;

    bool is_eof() const;
    bool is_digit(char c) const;
    bool is_alpha(char c) const; 
    bool is_alphanumeric(char c) const; 
    bool match(char expected);
    std::string get_lexeme(TokenType type) const; 
    void advance();
    char peek() const;
    char peek_next() const;
    void add_token(TokenType type);
    void scan_token();
    void string();
    void number();
    void identifier();
    
};

#endif