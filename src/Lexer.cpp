#include "../include/Lexer.hpp"
#include "../include/Logger.hpp"

const std::unordered_map<std::string, TokenType> Lexer::keywords{
    {"and", TokenType::AND},      {"or", TokenType::OR},
    {"nova", TokenType::NOVA},  {"probe", TokenType::PROBE},
    {"blackhole", TokenType::BLACKHOLE},    {"elprobe", TokenType::ELPROBE},
    {"void", TokenType::VOID}, {"cosmic", TokenType::COSMIC},
    {"mission", TokenType::MISSION},        {"navigate", TokenType::NAVIGATE},
    {"orbit", TokenType::ORBIT},  {"nil", TokenType::NIL},
    {"flare", TokenType::FLARE},  {"transmit", TokenType::TRANSMIT},
    {"supernova", TokenType::SUPERNOVA},  {"this", TokenType::THIS},
    {"atom", TokenType::ATOM},      {"lambda", TokenType::LAMBDA},
    {"eject", TokenType::EJECT},  {"warp", TokenType::WARP}};

Lexer::Lexer(std::string source) : source{std::move(source)} {
}

std::vector<Token> Lexer::scanTokens() {
    while (!isEOF()) {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::_EOF, "", line);
    return tokens;
}

void Lexer::scanToken() {
    char c = peek();
    advance();
    using enum TokenType;
    switch (c) {
    // 1 character lexemes.
    case '(':
        addToken(LEFT_PAREN);
        break;
    case ')':
        addToken(RIGHT_PAREN);
        break;
    case '{':
        addToken(LEFT_BRACE);
        break;
    case '}':
        addToken(RIGHT_BRACE);
        break;
    case '[':
        addToken(LEFT_BRACKET);
        break;
    case ']':
        addToken(RIGHT_BRACKET);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        addToken(DOT);
        break;
    case ';':
        addToken(SEMICOLON);
        break;
    case '*':
        addToken(STAR);
        break;

        // > 1 character lexemes.
    case '!':
        addToken(match('=') ? EXCLAMATION_EQUAL : EXCLAMATION);
        break;
    case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '-':
        addToken(match('-') ? MINUS_MINUS : MINUS);
        break;
    case '+':
        addToken(match('+') ? PLUS_PLUS : PLUS);
        break;
    case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;
    case '/':
        if (match('/')) {
            while (peek() != '\n' && !isEOF()) {
                advance();
            }
        } else {
            addToken(SLASH);
        }


    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;

    case '"':
        string();
        break;
    default:
        if (isDigit(c)) {
            number();
        } else if (isAlpha(c)) {
            identifier();
        } else {
            Error::addError(line, "", std::string("Unexpected character: '") + c + "'.");
        }
    }
}

void Lexer::identifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    const std::string text = source.substr(start, current - start);
    addToken(keywords.contains(text) ? keywords.at(text) : TokenType::IDENTIFIER);
}

void Lexer::number() {
    while (isDigit(peek())) {
        advance();
    }
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) {
            advance();
        }
    }

    addToken(TokenType::NUMBER);
}

void Lexer::string() {
    while (peek() != '"' && !isEOF()) {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }
    if (isEOF()) {
        Error::addError(line, "", "Unterminated string.");
        return;
    }

    advance();
    addToken(TokenType::STRING);
}

bool Lexer::match(char expected) {
    if (isEOF() || source.at(current) != expected) {
        return false;
    }
    current++;
    return true;
}

char Lexer::peek() const {
    return isEOF() ? '\0' : source.at(current);
}

char Lexer::peekNext() const {
    return (current + 1 >= source.length()) ? '\0' : source.at(current + 1);
}

bool Lexer::isAlpha(char c) const {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isEOF() const {
    return current >= source.size();
}

void Lexer::advance() {
    current++;
}

std::string Lexer::getLexeme(TokenType type) const {
    return (type == TokenType::STRING) ? source.substr(start + 1, current - start - 2) : source.substr(start, current - start);
}

void Lexer::addToken(const TokenType type) {
    tokens.emplace_back(type, getLexeme(type), line);
}
