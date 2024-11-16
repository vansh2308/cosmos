#ifndef COSMOS_SCANNER_SCANNER_H
#define COSMOS_SCANNER_SCANNER_H
#pragma once

#include <list>
#include <string>
#include <vector>

#include "Types/token.h"

namespace cosmos{
    using Types::Token;
    using Types::TokenType;


    class Scanner{
        public: 
        Scanner(const std::string& p_source);

        auto Tokenize() -> std::vector<Token>;

        private:
        auto is_at_end() -> bool;
        void tokeninze_one();
        void advance();
        auto match_next(char expected) -> bool;
        auto peek() -> char;
        auto peek_next() -> char;
        void skip_comment();
        void skip_block_comment();
        void eat_identifier();
        void eat_number();
        void eat_string();
        void add_token(TokenType t);

        const std::string& source;
        // ErrorReporter &eReporter;

        std::list<Token> tokens;
        size_t start = 0;
        size_t current = 0;
        int line = 1;
    };
}

#endif