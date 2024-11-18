
#include "evaluator.h"

#include <chrono>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <iterator>
#include <iostream>
#include <variant>
#include <utility>

#include "AST/prettyPrinter.h"
#include "Evaluator/objects.h"
#include "Types/literal.h"
#include "Types/token.h"

#define EXPECT_TRUE(x) __builtin_expect(static_cast<int64_t>(x), 1)
#define EXPECT_FALSE(x) __builtin_expect(static_cast<int64_t>(x), 0)

namespace cosmos::Evaluator{

    auto Evaluator::get_double(const Token& token, const csm_object& right) -> double {
        if(EXPECT_FALSE(!std::holds_alternative<double>(right)))
            // throw reportRuntimeError 

        return std::get<double>(right);
    }

    auto Evaluator::bind_instance(const func_shrd_ptr& method, csm_instance_shrd_ptr instance) -> func_shrd_ptr {
        auto env_to_restore = environManager.get_curr_env();
        environManager.set_curr_env(method->get_closure());
        environManager.create_new_environ();

        auto method_closure = environManager.get_curr_env();
        environManager.define("this", instance);
        environManager.set_curr_env(env_to_restore);

        return std::make_shared<FuncObj>(method->get_decl(), method->get_fn_name(), method_closure, method->get_is_method(), method->get_is_initalizer());
    }



    // --------------- Expression eval methods ---------------
    auto Evaluator::evaluate_binary_expr(const BinaryExprPtr& expr) -> csm_object {
        auto left = evaluate_expr(expr->left);
        auto right = evaluate_expr(expr->right);
        switch(expr->op.get_type()){
            case TokenType::COMMA: return right;
            case TokenType::BANG_EQUAL: return !are_equal(left, right);
            case TokenType::EQUAL_EQUAL: return are_equal(left, right);
            case TokenType::MINUS:
                return get_double(expr->op, left) - get_double(expr->op, right);
            case TokenType::SLASH: {
                double denominator = get_double(expr->op, right);
                if(EXPECT_FALSE(denominator == 0.0))
                    // division by zero error 
                return get_double(expr->op, left) / denominator;
            }
            case TokenType::STAR:
                return get_double(expr->op, left)* get_double(expr->op, right);
            case TokenType::LESS:
                return get_double(expr->op, left) < get_double(expr->op, right);
            
            case TokenType::LESS_EQUAL:
                  return get_double(expr->op, left) <= get_double(expr->op, right);
                case TokenType::GREATER:
                  return get_double(expr->op, left) > get_double(expr->op, right);
                case TokenType::GREATER_EQUAL:
                  return get_double(expr->op, left) >= get_double(expr->op, right);
                case TokenType::PLUS: {
                  if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                    return std::get<double>(left) + std::get<double>(right);
                  }
                  if (std::holds_alternative<std::string>(left) || std::holds_alternative<std::string>(right)) {
                    return get_object_string(left) + get_object_string(right);
                  }
                //   throw reportRuntimeError( eReporter, expr->op, "Operands to 'plus' must be numbers or strings; This is invalid: " + get_object_string(left) + " + " + get_object_string(right));
                }
                default:
                //   throw reportRuntimeError( eReporter, expr->op, "Attempted to apply invalid operator to binary expr: " + expr->op.getTypeString())
        }
    }




    auto Evaluator::evaluate_grouping_expr(const GroupingExprPtr& expr) -> csm_object {
        return evaluate_expr(expr->expression);
    }


    namespace {
        auto get_csm_object_from_string_literal(const Literal& str_literal) -> csm_object {
            const auto& str = std::get<std::string>(str_literal);
            if(str == "true") return csm_object(true);
            if(str == "false") return csm_object(false);
            if(str == "nil") return csm_object(nullptr);
            return csm_object(str);
        }
    } // namespace


    auto Evaluator::evaluate_literal_expr(const LiteralExprPtr& expr) -> csm_object {
        return expr->literalVal.has_value() ? std::holds_alternative<std::string>(expr->literalVal.value()) ? get_csm_object_from_string_literal(expr->literalVal.value()) : csm_object(std::get<double>(expr->literalVal.value())) : csm_object(nullptr);
    }


    auto Evaluator::evaluate_unary_expr(const UnaryExprPtr& expr) -> csm_object {
        csm_object right = evaluate_expr(expr->right);
        switch(expr->op.get_type()){
            case TokenType::BANG: return !is_true(right);
            case TokenType::MINUS: return -get_double(expr->op, right);
            case TokenType::PLUS_PLUS: return get_double(expr->op, right) + 1;
            case TokenType::MINUS_MINUS: return get_double(expr->op, right) - 1;
            default: 
                // throuw runtime error 
        }
    }

    auto Evaluator::evaluate_conditional_expr(const ConditionalExprPtr& expr) -> csm_object{
        if(is_true(evaluate_expr(expr->condition))) return evaluate_expr(expr->thenBranch);
        return evaluate_expr(expr->elseBranch);
    }


    auto Evaluator::evaluate_variable_expr(const VariableExprPtr& expr) -> csm_object {
        return environManager.get(expr->varName);
    }

    auto Evaluator::evaluate_assignment_expr(const AssignmentExprPtr& expr) -> csm_object {
        environManager.assign(expr->varName, evaluate_expr(expr->right));
        return environManager.get(expr->varName);
    }

    namespace {
        auto match(const Token& token, Types::TokenType tType) -> bool {
            return token.get_type() == tType;
        }

        auto do_postfix_op(const Token& op, const csm_object& val) -> csm_object {
            if(EXPECT_TRUE(std::holds_alternative<double>(val))) {
                double dval = std::get<double>(val);
                if(match(op, TokenType::PLUS_PLUS)) return csm_object(++dval);
                if(match(op, TokenType::MINUS_MINUS)) return csm_object(--dval);
            }

            // throw runtime errorr 
        }

    } // namespace


    auto Evaluator::evaluate_postfix_expr(const PostfixExprPtr& expr) -> csm_object {
        csm_object left_val = evaluate_expr(expr->left);
        if(EXPECT_TRUE(std::holds_alternative<VariableExprPtr>(expr->left))) {
            environManager.assign((std::get<VariableExprPtr>(expr->left))->varName, do_postfix_op(expr->op, left_val));
        }
        return left_val;
    }






} // namespace cosmos::Evaluator 



