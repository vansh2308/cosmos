#ifndef COSMOS_AST_NodeTypes_H
#define COSMOS_AST_NodeTypes_H
#pragma once 

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "Types/token.h"
#include "Types/literal.h"
#include "Types/uncopyable.h"

namespace cosmos::AST{
    using Types::Literal;
    using Types::OptionalLiteral;
    using Types::Token;
    using Types::TokenType;
    using Types::Uncopyable;

    struct BinaryExpr;
    struct GroupingExpr;
    struct LiteralExpr;
    struct UnaryExpr;
    struct ConditionalExpr;
    struct PostfixExpr;
    struct VariableExpr;
    struct AssignmentExpr;
    struct LogicalExpr;
    struct CallExpr;
    struct FuncExpr;
    struct GetExpr;
    struct SetExpr;
    struct ThisExpr;
    struct SuperExpr;

    using BinaryExprPtr = std::unique_ptr<BinaryExpr>;
    using GroupingExprPtr = std::unique_ptr<GroupingExpr>;
    using LiteralExprPtr = std::unique_ptr<LiteralExpr>;
    using UnaryExprPtr = std::unique_ptr<UnaryExpr>;
    using ConditionalExprPtr = std::unique_ptr<ConditionalExpr>;
    using PostfixExprPtr = std::unique_ptr<PostfixExpr>;
    using VariableExprPtr = std::unique_ptr<VariableExpr>;
    using AssignmentExprPtr = std::unique_ptr<AssignmentExpr>;
    using LogicalExprPtr = std::unique_ptr<LogicalExpr>;
    using CallExprPtr = std::unique_ptr<CallExpr>;
    using FuncExprPtr = std::unique_ptr<FuncExpr>;
    using GetExprPtr = std::unique_ptr<GetExpr>;
    using SetExprPtr = std::unique_ptr<SetExpr>;
    using ThisExprPtr = std::unique_ptr<ThisExpr>;
    using SuperExprPtr = std::unique_ptr<SuperExpr>;

    using ExprPtrVariant = std::variant<BinaryExprPtr, GroupingExprPtr, LiteralExprPtr, UnaryExprPtr,
        ConditionalExprPtr, PostfixExprPtr, VariableExprPtr, AssignmentExprPtr, LogicalExprPtr, CallExprPtr, FuncExprPtr,
        GetExprPtr, SetExprPtr, ThisExprPtr, SuperExprPtr>;



    struct ExprStmt;
    struct PrintStmt;
    struct BlockStmt;
    struct VarStmt;
    struct IfStmt;
    struct WhileStmt;
    struct ForStmt;
    struct FuncStmt;
    struct RetStmt;
    struct ClassStmt;

    using ExprStmtPtr = std::unique_ptr<ExprStmt>;
    using PrintStmtPtr = std::unique_ptr<PrintStmt>;
    using BlockStmtPtr = std::unique_ptr<BlockStmt>;
    using VarStmtPtr = std::unique_ptr<VarStmt>;
    using IfStmtPtr = std::unique_ptr<IfStmt>;
    using WhileStmtPtr = std::unique_ptr<WhileStmt>;
    using ForStmtPtr = std::unique_ptr<ForStmt>;
    using FuncStmtPtr = std::unique_ptr<FuncStmt>;
    using RetStmtPtr = std::unique_ptr<RetStmt>;
    using ClassStmtPtr = std::unique_ptr<ClassStmt>;


    using StmtPtrVariant = std::variant<ExprStmtPtr, PrintStmtPtr, BlockStmtPtr, VarStmtPtr,
        IfStmtPtr, WhileStmtPtr, ForStmtPtr, FuncStmtPtr, RetStmtPtr, ClassStmtPtr>;


    // Utilities to create Expression Pointer Variants  
    auto create_binary_EPV(ExprPtrVariant left, Token op, ExprPtrVariant right) -> ExprPtrVariant;
    auto create_unary_EPV(Token op, ExprPtrVariant right) -> ExprPtrVariant;
    auto create_grouping_EPV(ExprPtrVariant right) -> ExprPtrVariant;
    auto create_literal_EPV(OptionalLiteral literal) -> ExprPtrVariant;
    auto create_conditional_EPV(ExprPtrVariant condition, ExprPtrVariant then, ExprPtrVariant else_branch) -> ExprPtrVariant;
    auto create_postfix_EPV(ExprPtrVariant left, Token op) -> ExprPtrVariant;
    auto create_variable_EPV(Token var_name) -> ExprPtrVariant;
    auto create_assignment_EPV(Token var_name, ExprPtrVariant expr) -> ExprPtrVariant;
    auto create_logical_EPV(ExprPtrVariant left, Token op, ExprPtrVariant right) -> ExprPtrVariant;
    auto create_call_EPV(ExprPtrVariant calle, Token paren, std::vector<ExprPtrVariant> arguments) -> ExprPtrVariant;
    auto create_func_EPV(std::vector<Token> params, std::vector<StmtPtrVariant> fn_body) -> ExprPtrVariant;
    auto create_get_EPV(ExprPtrVariant expr, Token name) -> ExprPtrVariant;
    auto create_set_EPV(ExprPtrVariant expr, Token name, ExprPtrVariant value) -> ExprPtrVariant;
    auto create_this_EPV(Token keyword) -> ExprPtrVariant;
    auto create_super_EPV(Token keyword, Token method) -> ExprPtrVariant;


    // Utilities to create Statement Pointer Variants 
    auto create_expr_SPV(ExprPtrVariant expr) -> StmtPtrVariant;
    auto create_print_SPV(ExprPtrVariant expr) -> StmtPtrVariant;
    auto create_block_SPV(std::vector<StmtPtrVariant> statements) -> StmtPtrVariant;
    auto create_var_SPV(Token var_name, std::optional<ExprPtrVariant> initializer) -> StmtPtrVariant;
    auto create_if_SPV(ExprPtrVariant condition, StmtPtrVariant then_branch, std::optional<StmtPtrVariant> else_branch) -> StmtPtrVariant;
    auto create_while_SPV(ExprPtrVariant condition, StmtPtrVariant loopBody) -> StmtPtrVariant;
    auto create_for_SPV(std::optional<StmtPtrVariant> initializer, std::optional<ExprPtrVariant> condition, std::optional<ExprPtrVariant> increment, StmtPtrVariant loop_body) -> StmtPtrVariant;
    auto create_func_SPV(Token f_name, FuncExprPtr func_expr) -> StmtPtrVariant;
    auto create_ret_SPV(Token ret, std::optional<ExprPtrVariant> value) -> StmtPtrVariant;
    auto create_class_SPV(Token classname, std::optional<ExprPtrVariant> superclass, std::vector<StmtPtrVariant> methods) -> StmtPtrVariant;


    // Expression AST types 
    struct BinaryExpr final : public Uncopyable {
      ExprPtrVariant left;
      Token op;
      ExprPtrVariant right;
      BinaryExpr(ExprPtrVariant left, Token op, ExprPtrVariant right);
    };

    struct GroupingExpr final : public Uncopyable {
      ExprPtrVariant expression;
      explicit GroupingExpr(ExprPtrVariant expression);
    };

    struct LiteralExpr final : public Uncopyable {
      OptionalLiteral literalVal;
      explicit LiteralExpr(OptionalLiteral value);
    };

    struct UnaryExpr final : public Uncopyable {
      Token op;
      ExprPtrVariant right;
      UnaryExpr(Token op, ExprPtrVariant right);
    };

    struct ConditionalExpr final : public Uncopyable {
      ExprPtrVariant condition;
      ExprPtrVariant thenBranch;
      ExprPtrVariant elseBranch;
      ConditionalExpr(ExprPtrVariant condition, ExprPtrVariant thenBranch, ExprPtrVariant elseBranch);
    };

    struct PostfixExpr final : public Uncopyable {
      ExprPtrVariant left;
      Token op;
      PostfixExpr(ExprPtrVariant left, Token op);
    };

    struct VariableExpr final : public Uncopyable {
      Token varName;
      explicit VariableExpr(Token varName);
    };

    struct AssignmentExpr final : public Uncopyable {
      Token varName;
      ExprPtrVariant right;
      AssignmentExpr(Token varName, ExprPtrVariant right);
    };

    struct LogicalExpr final : public Uncopyable {
      ExprPtrVariant left;
      Token op;
      ExprPtrVariant right;
      LogicalExpr(ExprPtrVariant left, Token op, ExprPtrVariant right);
    };

    struct CallExpr final : public Uncopyable {
      ExprPtrVariant callee;
      Token paren;
      std::vector<ExprPtrVariant> arguments;
      CallExpr(ExprPtrVariant callee, Token paren, std::vector<ExprPtrVariant> arguments);
    };

    struct FuncExpr final : public Uncopyable {
      std::vector<Token> parameters;
      std::vector<StmtPtrVariant> body;
      FuncExpr(std::vector<Token> parameters, std::vector<StmtPtrVariant> body);
    };

    struct GetExpr final : public Uncopyable {
      ExprPtrVariant expr;
      Token name;
      GetExpr(ExprPtrVariant expr, Token name);
    };

    struct SetExpr final : public Uncopyable {
      ExprPtrVariant expr;
      Token name;
      ExprPtrVariant value;
      SetExpr(ExprPtrVariant expr, Token name, ExprPtrVariant value);
    };

    struct ThisExpr final : public Uncopyable {
      Token keyword;
      explicit ThisExpr(Token keyword);
    };

    struct SuperExpr final : public Uncopyable {
      Token keyword;
      Token method;
      explicit SuperExpr(Token keyword, Token method);
    };



    // Statement AST types 
    struct ExprStmt final : public Uncopyable {
        ExprPtrVariant expression;
        explicit ExprStmt(ExprPtrVariant expr);
    };

    struct PrintStmt final : public Uncopyable {
      ExprPtrVariant expression;
      explicit PrintStmt(ExprPtrVariant expression);
    };
    
    struct BlockStmt final : public Uncopyable {
      std::vector<StmtPtrVariant> statements;
      explicit BlockStmt(std::vector<StmtPtrVariant> statements);
    };
    
    struct VarStmt final : public Uncopyable {
      Token varName;
      std::optional<ExprPtrVariant> initializer;
      explicit VarStmt(Token varName, std::optional<ExprPtrVariant> initializer);
    };
    
    struct IfStmt final : public Uncopyable {
      ExprPtrVariant condition;
      StmtPtrVariant thenBranch;
      std::optional<StmtPtrVariant> elseBranch;
      explicit IfStmt(ExprPtrVariant condition, StmtPtrVariant thenBranch, std::optional<StmtPtrVariant> elseBranch);
    };
    
    struct WhileStmt final : public Uncopyable {
      ExprPtrVariant condition;
      StmtPtrVariant loopBody;
      explicit WhileStmt(ExprPtrVariant condition, StmtPtrVariant loopBody);
    };
    
    struct ForStmt final : public Uncopyable {
      std::optional<StmtPtrVariant> initializer;
      std::optional<ExprPtrVariant> condition;
      std::optional<ExprPtrVariant> increment;
      StmtPtrVariant loopBody;
      explicit ForStmt(std::optional<StmtPtrVariant> initializer, std::optional<ExprPtrVariant> condition,
            std::optional<ExprPtrVariant> increment, StmtPtrVariant loopBody);
    };
    
    struct FuncStmt : public Uncopyable {
      Token funcName;
      FuncExprPtr funcExpr;
      FuncStmt(Token funcName, FuncExprPtr funcExpr);
    };
    
    struct RetStmt : public Uncopyable {
      Token ret;
      std::optional<ExprPtrVariant> value;
      RetStmt(Token ret, std::optional<ExprPtrVariant> value);
    };
    
    struct ClassStmt : public Uncopyable {
      Token className;
      std::optional<ExprPtrVariant> superClass;
      std::vector<StmtPtrVariant> methods;
      ClassStmt(Token className, std::optional<ExprPtrVariant> superClass,
                std::vector<StmtPtrVariant> methods);
    };
    
}

#endif 