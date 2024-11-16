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


}

#endif 