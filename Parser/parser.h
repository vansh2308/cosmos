
#ifndef COSMOS_PARSER_PARSER_H
#define COSMOS_PARSER_PARSER_H
#pragma once

#include <exception>
#include <functional>
#include <iterator>
#include <vector>

#include "../AST/nodeTypes.h"
#include "../Types/token.h"


// Grammar production rules:
// program     → declaration* LOX_EOF;
// declaration → varDecl | funcDecl | classDecl | statement;
// varDecl     → "var" IDENTIFIER ("=" expression)? ";" ;
// classDecl   → "class" IDENTIFIER ("<" IDENTIFIER)? "{" funcDecl* "}" ;
// funcDecl    → IDENTIFIER funcBody;
// funcBody    → "(" parameters? ")" "{" declaration "}";
// statement   → exprStmt | printStmt | blockStmt | ifStmt | whileStmt |
// statement   → forStmt | returnStmt;
// exprStmt    → expression ';' ;
// printStmt   → "print" expression ';' ;
// blockStmt   → "{" declaration "}"
// ifStmt      → "if" "(" expression ")" statement ("else" statement)? ;
// whileStmt   → "while" "(" expression ")" statement;
// forStmt     → "for" "(" varDecl | exprStmnt | ";"
//                          expression? ";"
//                          expression? ")" statement;
// returnStmt  → "return" (expression)? ";";
// expression  → comma;
// comma       → assignment ("," assignment)*;
// arguments   → assignment  ( "," assignment )* ;
// assignment  → (call ".")? IDENTIFIER "=" assignment | condititional;
// conditional → logical_or ("?" expression ":" conditional)?;
// logical_or  → logical_and ("or" logical_and)*;
// logical_and → equality ("and" equality)*;
// equality    → comparison(("!=" | "==") comparison) *;
// comparison  → addition((">" | ">=" | "<" | "<=") addition) *;
// addition    → multiplication(("-" | "+") multiplication) *;
// multipli... → unary(("/" | "*") unary) *;
// unary       → ("!" | "-" | "--" | "++") unary | postfix;
// postfix     → call ("++" | "--")*;
// call        → primary ( "(" arguments? ")" | "." IDENTIFIER )*;
// primary     → NUMBER | STRING | "false" | "true" | "nil";
// primary     → "(" expression ")";
// primary     → IDENTIFIER;
// primary     → "fun" funcBody;
// primary     → "super" "." IDENTIFIER;
// Error Productions:
// primary     → ("!=" | "==") equality;
// primary     → (">" | ">=" | "<" | "<=") comparison;
// primary     → ("+")addition;
// primary     → ("/" | "*") multiplication;


namespace cosmos::Parser{
    using AST::ExprPtrVariant;
    using AST::StmtPtrVariant;

    // Recursive Descent Parser 
    class RDParser{
        public:
        // WIP: Add debugger 
        explicit RDParser(const std::vector<Types::Token>& tokens);

        class RDParserError : std::exception {}; // Exception Types 

        auto parse() -> std::vector<StmtPtrVariant>;


        private:
        // Grammar parsing fns 
        // Statement parsing fns 
        void program();
        auto declaration() -> std::optional<StmtPtrVariant>;
        auto varDecl() -> StmtPtrVariant;
        auto classDecl() -> StmtPtrVariant;
        auto funcDecl(const std::string& kind) -> StmtPtrVariant;
        auto funcBody(const std::string& kind) -> ExprPtrVariant;
        auto parameters() -> std::vector<Types::Token>;
        auto statement() -> StmtPtrVariant;
        auto exprStmt() -> StmtPtrVariant;
        auto printStmt() -> StmtPtrVariant;
        auto blockStmt() -> StmtPtrVariant;
        auto ifStmt() -> StmtPtrVariant;
        auto whileStmt() -> StmtPtrVariant;
        auto forStmt() -> StmtPtrVariant;
        auto returnStmt() -> StmtPtrVariant;

        // Expression Parsing
        auto expression() -> ExprPtrVariant;
        auto comma() -> ExprPtrVariant;
        auto assignment() -> ExprPtrVariant;
        auto conditional() -> ExprPtrVariant;
        auto logical_or() -> ExprPtrVariant;
        auto logical_and() -> ExprPtrVariant;
        auto equality() -> ExprPtrVariant;
        auto comparison() -> ExprPtrVariant;
        auto addition() -> ExprPtrVariant;
        auto multiplication() -> ExprPtrVariant;
        auto unary() -> ExprPtrVariant;
        auto postfix() -> ExprPtrVariant;
        auto call() -> ExprPtrVariant;
        auto arguments() -> std::vector<ExprPtrVariant>;
        auto primary() -> ExprPtrVariant;


        // Utils 
        void advance();
        void consume_or_error(Types::TokenType t_type, const std::string& error_msg);
        using parser_fn = ExprPtrVariant(RDParser::*)();
        auto consume_any_binary_exps(const std::initializer_list<Types::TokenType>& types, ExprPtrVariant expr, const parser_fn& f) -> ExprPtrVariant;
        auto consume_one_literal() -> ExprPtrVariant;
        auto consume_one_literal(const std::string& str) -> ExprPtrVariant;
        auto consume_grouping_expr() -> ExprPtrVariant;
        auto consume_postfix_expr(ExprPtrVariant expr) -> ExprPtrVariant;
        void consume_semicolon_or_error();
        auto consume_super() -> ExprPtrVariant;
        auto consume_unary_expr() -> ExprPtrVariant;
        auto consume_var_expr() -> ExprPtrVariant;
        auto error(const std::string& err_msg) -> RDParserError;
        [[nodiscard]] auto get_current_token_type() const -> Types::TokenType;
        auto get_token_and_advance() -> Types::Token;
        [[nodiscard]] auto is_at_end() const ->  bool;
        [[nodiscard]] auto match(const std::initializer_list<Types::TokenType>& types) const -> bool;
        [[nodiscard]] auto match(Types::TokenType type) const -> bool;
        [[nodiscard]] auto match_next(Types::TokenType type) -> bool;
        [[nodiscard]] auto peek() const -> Types::Token;
        void report_error(const std::string& msg);
        void synchronize();
        void throw_on_error_production(const std::initializer_list<Types::TokenType>& types, const parser_fn& f);
        void throw_on_error_productions();


        const std::vector<Types::Token>& tokens;
        std::vector<Types::Token>::const_iterator current_iter;
        
        // WIP: Add debugger 
        std::vector<StmtPtrVariant> statements;
        
        static const int MAX_ARGS = 255;

    };

}

#endif