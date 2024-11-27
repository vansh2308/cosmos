
#include "../include/Lexer.hpp"

const std::unordered_map<std::string, TokenType> Lexer::keywords{
    {"and", TokenType::AND},                {"or", TokenType::OR},
    {"nova", TokenType::NOVA},              {"probe", TokenType::PROBE},
    {"blackhole", TokenType::BLACKHOLE},    {"elprobe", TokenType::ELPROBE},
    {"void", TokenType::VOID},              {"cosmic", TokenType::COSMIC},
    {"mission", TokenType::MISSION},        {"navigate", TokenType::NAVIGATE},
    {"orbit", TokenType::ORBIT},            {"nil", TokenType::NIL},
    {"flare", TokenType::FLARE},            {"transmit", TokenType::TRANSMIT},
    {"supernova", TokenType::SUPERNOVA},    {"this", TokenType::THIS},
    {"atom", TokenType::ATOM},              {"lambda", TokenType::LAMBDA},
    {"eject", TokenType::EJECT},            {"warp", TokenType::WARP}
};


Lexer::Lexer(std::string source) : source(std::move(source)) {}

std::vector<Token> Lexer::scan_tokens(){
    while(!is_eof()){
        start = current;
        scan_token();
    }

    tokens.emplace_back(TokenType::_EOF, "", line);
    return tokens;
}

void Lexer::scan_token(){
    char c = peek();
    advance();
    using enum TokenType;
    switch(c){
    case '(':
        add_token(LEFT_PAREN);
        break;
    case ')':
        add_token(RIGHT_PAREN);
        break;
    case '{':
        add_token(LEFT_BRACE);
        break;
    case '}':
        add_token(RIGHT_BRACE);
        break;
    case '[':
        add_token(LEFT_BRACKET);
        break;
    case ']':
        add_token(RIGHT_BRACKET);
        break;
    case ',':
        add_token(COMMA);
        break;
    case '.':
        add_token(DOT);
        break;
    case ';':
        add_token(SEMICOLON);
        break;
    case '*':
        add_token(STAR);
        break;

    case '!':
        add_token(match('=') ? EXCLAMATION_EQUAL : EXCLAMATION);
        break;
    case '=':
        add_token(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '-':
        add_token(match('-') ? MINUS_MINUS : MINUS);
        break;
    case '+':
        add_token(match('+') ? PLUS_PLUS : PLUS);
        break;
    case '<':
        add_token(match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        add_token(match('=') ? GREATER_EQUAL : GREATER);
        break;
    case '/':
        if (match('/')) {
            while (peek() != '\n' && !is_eof()) {
                advance();
            }
        } else {
            add_token(SLASH);
        }
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;

        // Literals.
    case '"':
        string();
        break;
    default:
        if (is_digit(c)) {
            number();
        }
        else if (is_alpha(c)) {
            identifier();
        }
        else {
            // Error::addError(line, "", std::string("Unexpected character: '") + c + "'.");
        }
    }
}


void Lexer::identifier(){
    while(is_alphanumeric(peek())){
        advance();
    }

    const std::string text = source.substr(start, current-start);
    add_token(keywords.contains(text) ? keywords.at(text) : TokenType::IDENTIFIER);
}


void Lexer::number(){
    while(is_digit(peek())){
        advance();
    }
    if(peek() == '.' && is_digit(peek_next())){
        // consume '.'
        advance();
        while(is_digit(peek())){
            advance();
        }
    }
    add_token(TokenType::NUMBER);
}


void Lexer::string(){
    while(peek() != '"' && !is_eof()){
        if(peek() == '\n'){
            line++;
        }
        advance();
    }

    if(is_eof()){
        // WIP: runtime error unterminated string 
        return;
    }

    advance();
    add_token(TokenType::STRING);
}



bool Lexer::match(char expected){
    if(is_eof() || source.at(current) != expected){
        return false;
    }
    current++;
    return true;
}

char Lexer::peek() const {
    return is_eof() ? '\0' : source.at(current);
}

char Lexer::peek_next() const {
    return (current+1 >= source.length()) ? '\0' : source.at(current+1);
}

bool Lexer::is_alpha(char c) const{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}

bool Lexer::is_alphanumeric(char c) const  {
    return is_alpha(c) || is_digit(c);
}

bool Lexer::is_digit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::is_eof() const {
    return current >= source.size();
}

void Lexer::advance(){
    current++;
}

std::string Lexer::get_lexeme(TokenType type) const {
    return (type == TokenType::STRING) ? source.substr(start+1, current-start-2) : source.substr(start, current - start);
}

void Lexer::add_token(const TokenType type){
    tokens.emplace_back(type, get_lexeme(type), line);
}

