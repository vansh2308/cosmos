
#include "Scanner/scanner.h"

#include <map>

#include "Types/token.h"

namespace cosmos{
    using Types::Literal;
    using Types::OptionalLiteral;
    using Types::Token;
    using Types::TokenType;
    
    namespace {
        auto is_alpha(char c) -> bool {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
        }
        auto is_digit(char c) -> bool { return c >= '0' && c <= '9'; }

        auto is_alphanumeric(char c) -> bool { return is_alpha(c) || is_digit(c); }

        auto reserved_or_identifier(const std::string& str) -> TokenType {
            static const std::map<std::string, TokenType> lookup_table {
                {"and", TokenType::AND},
                {"blackhole", TokenType::BLACKHOLE},
                {"mission", TokenType::MISSION},
                {"probe", TokenType::PROBE},
                {"or", TokenType::OR},
                {"transmit", TokenType::TRANSMIT},
                {"this", TokenType::THIS},
                {"atom", TokenType::ATOM}, 
                {"galaxy", TokenType::GALAXY},
                {"void", TokenType::VOID},
                {"for", TokenType::NAVIGATE},
                {"nil", TokenType::NIL},
                {"flare", TokenType::FLARE},
                {"super", TokenType::SUPER},
                {"cosmic", TokenType::COSMIC},
                {"orbit", TokenType::ORBIT}
            };

            auto iter = lookup_table.find(str);
            if(iter == lookup_table.end()){
                return TokenType::IDENTIFIER;
            }
            return iter->second;
        }

        auto get_lexeme(const std::string& source, size_t start, size_t end) -> std::string {
            return source.substr(start, end);
        }

        auto make_optional_literal(TokenType t, const std::string& lexeme) -> OptionalLiteral {
            switch(t){
                case TokenType::NUMBER:
                return Types::make_optional_literal(std::stod(lexeme));

                case TokenType::STRING:
                return Types::make_optional_literal(lexeme.substr(1, lexeme.size()-2));

                default: return std::nullopt;
            }
        }
    } // namespace

    // WIP: add eReporter 
    Scanner::Scanner(const std::string& p_source): source(p_source) {}

    void Scanner::add_token(TokenType t){
        const std::string lexeme = get_lexeme(source, start, current-start);
        tokens.emplace_back(t, lexeme, make_optional_literal(t, lexeme), line);
    }

    void Scanner::advance() { ++current; }

    void Scanner::skip_block_comment(){
        int nesting = 1;
        while(nesting > 0){
            if(peek() == '\0'){
                // WIP: ereporter 
                return;
            }
            if(peek() == '/' && peek_next() == '*'){
                advance();
                ++nesting;
            } else if (peek() == '*' && peek_next() == '/'){
                advance();
                --nesting;
            } else if (peek() == '\n'){
                ++line;
            }

            advance();
        }
    }


    void Scanner::skip_comment(){
        while(!is_at_end() && peek() != '\n'){ advance(); }
    }

    void Scanner::eat_identifier(){
        while(is_alphanumeric(peek())) advance();
    }

    void Scanner::eat_number(){
        while(is_digit(peek())) advance();

        if(peek() == '.' && is_digit(peek_next())){
            advance();
            while(is_digit(peek())) advance();
        }
    }

    void Scanner::eat_string(){
        while(!is_at_end() && peek() != '"'){
            if(peek() == '\n') ++line;
            advance();
        }
        if(is_at_end()){
            // WIP: ereporter
        } else {
            advance();
        }
    }

    auto Scanner::is_at_end() -> bool{ return current >= source.size(); }

    auto Scanner::match_next(char expected) -> bool {
        bool next_matches = (peek() == expected);
        if(next_matches) advance();
        return next_matches;
    } 

    auto Scanner::peek() -> char {
        if(is_at_end) return '\0';
        return source[current];
    }

    auto Scanner::peek_next() -> char {
        if((current+1) >= source.size())  return '\0';
        return source[current+1];
    }

    void Scanner::tokeninze_one(){
        char c = peek();
        advance();
        switch(c){
            case '(': add_token(TokenType::LEFT_PAREN); break;
            case ')': add_token(TokenType::RIGHT_PAREN); break;
            case '{': add_token(TokenType::LEFT_BRACE); break;
            case '}': add_token(TokenType::RIGHT_BRACE); break;
            case ',': add_token(TokenType::COMMA); break;
            case ':': add_token(TokenType::COLON); break;
            case '.': add_token(TokenType::DOT); break;
            case '?': add_token(TokenType::QUESTION); break;
            case ';': add_token(TokenType::SEMICOLON); break;
            case '*': add_token(TokenType::STAR); break;
            case '!':
              add_token(match_next('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
              break;
            case '=':
              add_token(match_next('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
              break;
            case '>':
              add_token(match_next('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
              break;
            case '<':
              add_token(match_next('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
              break;
            case '-':
              add_token(match_next('-') ? TokenType::MINUS_MINUS : TokenType::MINUS);
              break;
            case '+':
              add_token(match_next('+') ? TokenType::PLUS_PLUS : TokenType::PLUS);
              break;
            case '/':
              if (match_next('/'))
                skip_comment();
              else if (match_next('*'))
                skip_block_comment();
              else
                add_token(TokenType::SLASH);
              break;
            case ' ':
            case '\t':
            case '\r': break;  // whitespace
            case '\n': ++line; break;
            case '"':
              eat_string();
              add_token(TokenType::STRING);
              break;

            default:
                if(is_digit(c)){
                    eat_number();
                    add_token(TokenType::NUMBER);
                } else if(is_alpha(c)){
                    eat_identifier();
                    const std::string identifier = get_lexeme(source, start, current-start);
                    add_token(reserved_or_identifier(identifier));
                } else {
                    std::string message = "Unexpected character: ";
                    message.append(1, static_cast<char>(c));
                    // WIP: ereporter 
                }
                break;
        }
    }


    auto Scanner::Tokenize() -> std::vector<Token> {
        while(!is_at_end()){
            start = current;
            tokeninze_one();
        }

        tokens.emplace_back(TokenType::CSM_EOF, "", std::nullopt, line);
        std::vector<Token> tokens_vec;
        std::move(tokens.begin(), tokens.end(), std::back_inserter(tokens_vec));
        return tokens_vec;
    }

} // namespace cosmos
