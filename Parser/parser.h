
#ifndef COSMOS_PARSER_PARSER_H
#define COSMOS_PARSER_PARSER_H
#pragma once

#include <exception>
#include <functional>
#include <iterator>
#include <vector>

#include "AST/nodeTypes.h"
#include "Types/token.h"


// Grammar production rules:
// program     → declaration* LOX_EOF;
// declaration → varDecl | funcDecl | classDecl | statement;
// varDecl     → "var" IDENTIFIER ("=" expression)? ";" ;
// classDecl   → "class" IDENTIFIER ("<" IDENTIFIER)? "{" funcDecl* "}" ;
// funcDecl    → IDENTIFIER funcBody;
// funcBody     → "(" parameters? ")" "{" declaration "}";
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


    

}

#endif