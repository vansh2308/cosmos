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


}

#endif 