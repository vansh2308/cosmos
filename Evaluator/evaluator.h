
#ifndef COSMOS_EVALUATOR_EVALUATOR__H
#define COSMOS_EVALUATOR_EVALUATOR__H
#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "AST/nodeTypes.h"
#include "Evaluator/environment.h"
#include "Evaluator/objects.h"
#include "Types/token.h"
#include "Types/uncopyable.h"

namespace cosmos::Evaluator {
    using AST::AssignmentExprPtr;
    using AST::BinaryExprPtr;
    using AST::CallExprPtr;
    using AST::ConditionalExprPtr;
    using AST::ExprPtrVariant;
    using AST::FuncExprPtr;
    using AST::FuncStmtPtr;
    using AST::GetExprPtr;
    using AST::GroupingExprPtr;
    using AST::LiteralExprPtr;
    using AST::LogicalExprPtr;
    using AST::PostfixExprPtr;
    using AST::SetExprPtr;
    using AST::SuperExprPtr;
    using AST::ThisExprPtr;
    using AST::UnaryExprPtr;
    using AST::VariableExprPtr;

    using AST::BlockStmtPtr;
    using AST::ClassStmtPtr;
    using AST::ExprStmtPtr;
    using AST::ForStmtPtr;
    using AST::IfStmtPtr;
    using AST::PrintStmtPtr;
    using AST::RetStmtPtr;
    using AST::StmtPtrVariant;
    using AST::VarStmtPtr;
    using AST::WhileStmtPtr;

    // WIP: using ErrorsAndDebug::ErrorReporter;

    using Types::Literal;
    using Types::Token;
    using Types::TokenType;

    // WIP: using ErrorsAndDebug::ErrorReporter;

    class Evaluator {
    public:
        // explicit Evaluator(ErrorReporter &eReporter);
        explicit Evaluator();
        auto evaluate_expr(const ExprPtrVariant &expr) -> csm_object;
        auto evaluate_stmt(const AST::StmtPtrVariant &stmt) -> std::optional<csm_object>;
        auto evaluate_stmts(const std::vector<AST::StmtPtrVariant> &stmts) -> std::optional<csm_object>;

    private:
        // evaluation functions for Expr types
        auto evaluate_binary_expr(const BinaryExprPtr &expr) -> csm_object;
        auto evaluate_grouping_expr(const GroupingExprPtr &expr) -> csm_object;
        static auto evaluate_literal_expr(const LiteralExprPtr &expr) -> csm_object;
        auto evaluate_unary_expr(const UnaryExprPtr &expr) -> csm_object;
        auto evaluate_conditional_expr(const ConditionalExprPtr &expr) -> csm_object;
        auto evaluate_postfix_expr(const PostfixExprPtr &expr) -> csm_object;
        auto evaluate_variable_expr(const VariableExprPtr &expr) -> csm_object;
        auto evaluate_assignment_expr(const AssignmentExprPtr &expr) -> csm_object;
        auto evaluate_logical_expr(const LogicalExprPtr &expr) -> csm_object;
        auto evaluate_call_expr(const CallExprPtr &expr) -> csm_object;
        auto evaluate_func_expr(const FuncExprPtr &expr) -> csm_object;
        auto evaluate_get_expr(const GetExprPtr &expr) -> csm_object;
        auto evaluate_set_expr(const SetExprPtr &expr) -> csm_object;
        auto evaluate_this_expr(const ThisExprPtr &expr) -> csm_object;
        auto evaluate_super_expr(const SuperExprPtr &expr) -> csm_object;

        // evaluation functions for Stmt types
        auto evaluate_expr_stmt(const ExprStmtPtr &stmt) -> std::optional<csm_object>;
        auto evaluate_print_stmt(const PrintStmtPtr &stmt) -> std::optional<csm_object>;
        auto evaluate_block_stmt(const BlockStmtPtr &stmt) -> std::optional<csm_object>;
        auto evaluate_var_stmt(const VarStmtPtr &stmt) -> std::optional<csm_object>;
        auto evaluate_if_stmt(const IfStmtPtr &stmt) -> std::optional<csm_object>;
        auto evaluate_while_stmt(const WhileStmtPtr &stmt) -> std::optional<csm_object>;
        auto evalaute_for_stmt(const ForStmtPtr &stmt) -> std::optional<csm_object>;
        auto evalaute_func_stmt(const FuncStmtPtr &stmt) -> std::optional<csm_object>;
        auto evaluate_ret_stmt(const RetStmtPtr &stmt) -> std::optional<csm_object>;
        auto evaluate_class_stmt(const ClassStmtPtr &stmt) -> std::optional<csm_object>;

        // throws RuntimeError if right isn't a double
        auto get_double(const Token &token, const csm_object &right) -> double;
        auto bind_instance(const func_shrd_ptr &method, csm_instance_shrd_ptr instance) -> func_shrd_ptr;

        // ErrorReporter &eReporter;
        EnvironmentManager environManager;

        static const int MAX_RUNTIME_ERR = 20;
        int num_runtime_err = 0;
    };

} // namespace cosmos::Evaluator
#endif 