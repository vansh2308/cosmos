
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



    } // namespace
    

}
