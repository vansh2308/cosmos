#include "Evaluator/evaluator.h"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>

#include "AST/prettyPrinter.h"
#include "Evaluator/objects.h"
#include "Types/literal.h"
#include "Types/token.h"

#define EXPECT_TRUE(x) __builtin_expect(static_cast<int64_t>(x), 1)
#define EXPECT_FALSE(x) __builtin_expect(static_cast<int64_t>(x), 0)

namespace cosmos::Evaluator{
    // throws RuntimeError if right isn't a double
    auto Evaluator::get_double(const Token &token, const csm_object &right) -> double{
        if (EXPECT_FALSE(!std::holds_alternative<double>(right)))
            // throw reportRuntimeError( eReporter, token, "Attempted to perform arithmetic operation on non-numeric literal " + getObjectString(right));
            return std::get<double>(right);
    }

    auto Evaluator::bind_instance(const func_shrd_ptr &method, csm_instance_shrd_ptr instance) -> func_shrd_ptr{
        auto environToRestore = environManager.get_curr_env();
        environManager.set_curr_env(method->get_closure());
        environManager.create_new_environ();
        auto methodClosure = environManager.get_curr_env();
        environManager.define("this", instance);
        environManager.set_curr_env(environToRestore);
        return std::make_shared<FuncObj>(method->get_decl(), method->get_fn_name(), methodClosure, method->get_is_method(), method->get_is_initalizer());
    }

    // ------------------ Expression Evaluation Methods ------------------
    auto Evaluator::evaluate_binary_expr(const BinaryExprPtr &expr) -> csm_object {
        auto left = evaluate_expr(expr->left);
        auto right = evaluate_expr(expr->right);
        switch (expr->op.get_type()) {
        case TokenType::COMMA:
            return right;
        case TokenType::BANG_EQUAL:
            return !are_equal(left, right);
        case TokenType::EQUAL_EQUAL:
            return are_equal(left, right);
        case TokenType::MINUS:
            return get_double(expr->op, left) - get_double(expr->op, right);
        case TokenType::SLASH: {
            double denominator = get_double(expr->op, right);
            if (EXPECT_FALSE(denominator == 0.0))
                // throw reportRuntimeError(eReporter, expr->op, "Division by zero is illegal");
                return get_double(expr->op, left) / denominator;
        }
        case TokenType::STAR:
            return get_double(expr->op, left) * get_double(expr->op, right);
        case TokenType::LESS:
            return get_double(expr->op, left) < get_double(expr->op, right);
        case TokenType::LESS_EQUAL:
            return get_double(expr->op, left) <= get_double(expr->op, right);
        case TokenType::GREATER:
            return get_double(expr->op, left) > get_double(expr->op, right);
        case TokenType::GREATER_EQUAL:
            return get_double(expr->op, left) >= get_double(expr->op, right);
        case TokenType::PLUS: {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
            {
                return std::get<double>(left) + std::get<double>(right);
            }
            if (std::holds_alternative<std::string>(left) || std::holds_alternative<std::string>(right))
            {
                return get_object_string(left) + get_object_string(right);
            }
            //   throw reportRuntimeError( eReporter, expr->op, "Operands to 'plus' must be numbers or strings; This is invalid: " + getObjectString(left) + " + " + getObjectString(right));
        }
        default:
            //   throw reportRuntimeError( eReporter, expr->op, "Attempted to apply invalid operator to binary expr: " + expr->op.getTypeString());
        }
    }

    auto Evaluator::evaluate_grouping_expr(const GroupingExprPtr &expr) -> csm_object { return evaluate_expr(expr->expression); }

    namespace{
        auto get_csm_object_from_string_literal(const Literal &strLiteral) -> csm_object {
            const auto &str = std::get<std::string>(strLiteral);
            if (str == "true")
                return csm_object(true);
            if (str == "false")
                return csm_object(false);
            if (str == "nil")
                return csm_object(nullptr);
            return csm_object(str);
        };
    } // namespace

    auto Evaluator::evaluate_literal_expr(const LiteralExprPtr &expr) -> csm_object {
        return expr->literalVal.has_value() ? std::holds_alternative<std::string>(expr->literalVal.value()) ? get_csm_object_from_string_literal(expr->literalVal.value()) : csm_object(std::get<double>(expr->literalVal.value())) : csm_object(nullptr);
    }

    auto Evaluator::evaluate_unary_expr(const UnaryExprPtr &expr) -> csm_object {
        csm_object right = evaluate_expr(expr->right);
        switch (expr->op.get_type()) {
        case TokenType::BANG:
            return !is_true(right);
        case TokenType::MINUS:
            return -get_double(expr->op, right);
        case TokenType::PLUS_PLUS:
            return get_double(expr->op, right) + 1;
        case TokenType::MINUS_MINUS:
            return get_double(expr->op, right) - 1;
        default:
            //   throw reportRuntimeError( eReporter, expr->op, "Illegal unary expression: " + expr->op.get_lexeme() + getObjectString(right));
        }
    }

    auto Evaluator::evaluate_conditional_expr(const ConditionalExprPtr &expr) -> csm_object {
        if (is_true(evaluate_expr(expr->condition)))
            return evaluate_expr(expr->thenBranch);
        return evaluate_expr(expr->elseBranch);
    }

    auto Evaluator::evaluate_variable_expr(const VariableExprPtr &expr) -> csm_object { return environManager.get(expr->varName); }

    auto Evaluator::evaluate_assignment_expr(const AssignmentExprPtr &expr) -> csm_object {
        environManager.assign(expr->varName, evaluate_expr(expr->right));
        return environManager.get(expr->varName);
    }

    namespace {
        auto match(const Token &token, Types::TokenType tType) -> bool {
            return token.get_type() == tType;
        }

        auto doPostfixOp(const Token &op, const csm_object &val) -> csm_object {
            if (EXPECT_TRUE(std::holds_alternative<double>(val))) {
                double dVal = std::get<double>(val);
                if (match(op, TokenType::PLUS_PLUS))
                    return csm_object(++dVal);
                if (match(op, TokenType::MINUS_MINUS))
                    return csm_object(--dVal);
            }
            //   throw ErrorsAndDebug::RuntimeError();
        }
    } // namespace

    auto Evaluator::evaluate_postfix_expr(const PostfixExprPtr &expr) -> csm_object {
        csm_object leftVal = evaluate_expr(expr->left);
        if (EXPECT_TRUE(std::holds_alternative<VariableExprPtr>(expr->left))) {
            environManager.assign((std::get<VariableExprPtr>(expr->left))->varName, doPostfixOp(expr->op, leftVal));
        }
        return leftVal;
    }

    auto Evaluator::evaluate_logical_expr(const LogicalExprPtr &expr) -> csm_object {
        csm_object leftVal = evaluate_expr(expr->left);
        if (expr->op.get_type() == TokenType::OR)
            return is_true(leftVal) ? leftVal : evaluate_expr(expr->right);
        if (expr->op.get_type() == TokenType::AND)
            return !is_true(leftVal) ? leftVal : evaluate_expr(expr->right);

        //   throw reportRuntimeError(eReporter, expr->op, "Illegal logical operator: " + expr->op.get_lexeme());
    }

    auto Evaluator::evaluate_call_expr(const CallExprPtr &expr) -> csm_object {
        csm_object callee = evaluate_expr(expr->callee);

#ifdef EVAL_DEBUG
//   ErrorsAndDebug::debugPrint("evaluateCallExpr called. Callee:" + getObjectString(callee));
#endif // EVAL_DEBUG

        if (EXPECT_FALSE(std::holds_alternative<built_in_func_shrd_ptr>(callee))) {
            return std::get<built_in_func_shrd_ptr>(callee)->run();
        }

        csm_object instanceOrNull = ([&]() -> csm_object {
            if (EXPECT_FALSE(std::holds_alternative<csm_class_shrd_ptr>(callee)))
              return csm_object(
                  std::make_shared<CsmInstance>(std::get<csm_class_shrd_ptr>(callee)));
            return csm_object(nullptr); })();

        const func_shrd_ptr funcObj = ([&]() -> func_shrd_ptr {
                                           if (std::holds_alternative<csm_class_shrd_ptr>(callee)) {
                                               auto instance = std::get<csm_instance_shrd_ptr>(instanceOrNull);
                                               try {
                                                   return bind_instance(std::get<func_shrd_ptr>(instance->get("init")), instance);

                                                   //  WIP:  } catch (const ErrorsAndDebug::RuntimeError& e) {
                                               }
                                               catch (std::exception &e) {
                                                   return nullptr;
                                               }
                                           }

                                           if (EXPECT_TRUE(std::holds_alternative<func_shrd_ptr>(callee)))
                                               return std::get<func_shrd_ptr>(callee);

                                           // throw reportRuntimeError(eReporter, expr->paren, "Attempted to invoke a non-function");
                                       })();

        if (funcObj.get() == nullptr) {
            return instanceOrNull;
        }

        // Throw error if arity doesn't match the number of arguments supplied
        if (size_t arity = funcObj->arity(), numArgs = expr->arguments.size(); EXPECT_FALSE(arity != numArgs)){
            // throw reportRuntimeError(eReporter, expr->paren, "Expected " + std::to_string(arity) + " arguments. Got " + std::to_string(numArgs) + " arguments. ");

        }

        std::vector<csm_object> evaldArgs;
        for (const auto &arg : expr->arguments)
            evaldArgs.push_back(evaluate_expr(arg));


        auto environToRestore = environManager.get_curr_env();
        environManager.set_curr_env(funcObj->get_closure());
        environManager.create_new_environ();

        { // Define each parameter with evaluated argument
            const auto &params = funcObj->get_params();
            auto param = params.begin();
            auto arg = evaldArgs.begin();
            for (; param != params.end() && arg != evaldArgs.end(); ++param, ++arg) {
                environManager.define(*param, *arg);
            }
        }

#ifdef EVAL_DEBUG
        ErrorsAndDebug::debugPrint("FnBodyStmts:");
        for (const auto &stmt :
             AST::PrettyPrinter::toString(funcObj->getFnBodyStmts()))
            ErrorsAndDebug::debugPrint(stmt);
#endif // EVAL_DEBUG


        std::optional<csm_object> fnRet = evaluate_stmts(funcObj->get_fn_body_stmts());


        if (!funcObj->get_is_method())
            environManager.discard_environs_till(funcObj->get_closure());
        else
            environManager.discard_environs_till(funcObj->get_closure()->get_parent_env());

        environManager.set_curr_env(environToRestore);

        if (fnRet.has_value()) {
            if (EXPECT_FALSE(funcObj->get_is_initalizer()))
                //   throw ErrorsAndDebug::reportRuntimeError( eReporter, expr->paren, "Initializer can't return a value other than 'this'");
                return fnRet.value();
        }
        return instanceOrNull;
    }

    auto Evaluator::evaluate_func_expr(const FuncExprPtr &expr) -> csm_object{
        auto closure = environManager.get_curr_env();
        environManager.create_new_environ();
        return std::make_shared<FuncObj>(expr, "CsmAnonFuncDoNotUseThisNameAADWAED", std::move(closure));
    }

    auto Evaluator::evaluate_get_expr(const GetExprPtr &expr) -> csm_object {
        csm_object instObj = evaluate_expr(expr->expr);
        if (EXPECT_FALSE(!std::holds_alternative<csm_instance_shrd_ptr>(instObj)))
            // throw reportRuntimeError(eReporter, expr->name, "Only instances have properties");
            try {
                csm_object property = std::get<csm_instance_shrd_ptr>(instObj)->get(expr->name.get_lexeme());
                if (std::holds_alternative<func_shrd_ptr>(property)) {
                    property = csm_object(bind_instance(std::get<func_shrd_ptr>(property), std::get<csm_instance_shrd_ptr>(instObj)));
                }
                return property;

                //   WIP: } catch (const ErrorsAndDebug::RuntimeError& e) {
            }
            catch (std::exception &e) {
                // throw ErrorsAndDebug::reportRuntimeError( eReporter, expr->name, "Attempted to access undefined property: " + expr->name.get_lexeme() + " on " + std::get<CsmInstanceShrdPtr>(instObj)->toString());
            }
    }

    auto Evaluator::evaluate_set_expr(const AST::SetExprPtr &expr) -> csm_object {
        csm_object object = evaluate_expr(expr->expr);
        if (EXPECT_FALSE(!std::holds_alternative<csm_instance_shrd_ptr>(object))){
            // throw ErrorsAndDebug::reportRuntimeError(eReporter, expr->name, "Only instances have fields.");
        }
        csm_object value = evaluate_expr(expr->value);
        std::get<csm_instance_shrd_ptr>(object)->set(expr->name.get_lexeme(), value);
        return value;
    }

    auto Evaluator::evaluate_this_expr(const ThisExprPtr &expr) -> csm_object {
        return environManager.get(expr->keyword);
    }

    auto Evaluator::evaluate_super_expr(const SuperExprPtr &expr) -> csm_object {
        csm_class_shrd_ptr superClass = std::get<csm_class_shrd_ptr>(environManager.get(expr->keyword));
        auto optionalMethod = superClass->find_method(expr->method.get_lexeme());
        if (!optionalMethod.has_value())
            // throw ErrorsAndDebug::reportRuntimeError( eReporter, expr->keyword, "Attempted to access undefined property " + expr->keyword.get_lexeme() + " on super.");

            return bind_instance(std::get<func_shrd_ptr>(optionalMethod.value()), std::get<csm_instance_shrd_ptr>(environManager.get(Token(TokenType::THIS, "this"))));
    }

    auto Evaluator::evaluate_expr(const ExprPtrVariant &expr) -> csm_object {
        switch (expr.index()) {
        case 0: // BinaryExprPtr
            return evaluate_binary_expr(std::get<0>(expr));
        case 1: // GroupingExprPtr
            return evaluate_grouping_expr(std::get<1>(expr));
        case 2: // LiteralExprPtr
            return evaluate_literal_expr(std::get<2>(expr));
        case 3: // UnaryExprPtr
            return evaluate_unary_expr(std::get<3>(expr));
        case 4: // ConditionalExprPtr
            return evaluate_conditional_expr(std::get<4>(expr));
        case 5: // PostfixExprPtr
            return evaluate_postfix_expr(std::get<5>(expr));
        case 6: // VariableExprPtr
            return evaluate_variable_expr(std::get<6>(expr));
        case 7: // AssignmentExprPtr
            return evaluate_assignment_expr(std::get<7>(expr));
        case 8: // LogicalExprPtr
            return evaluate_logical_expr(std::get<8>(expr));
        case 9: // CallExprPtr
            return evaluate_call_expr(std::get<9>(expr));
        case 10: // FuncExprPtr
            return evaluate_func_expr(std::get<10>(expr));
        case 11: // GetExprPtr
            return evaluate_get_expr(std::get<11>(expr));
        case 12: // SetExprPtr
            return evaluate_set_expr(std::get<12>(expr));
        case 13: // ThisExprPtr
            return evaluate_this_expr(std::get<13>(expr));
        case 14: // SuperExprPtr
            return evaluate_super_expr(std::get<14>(expr));
        default:
            static_assert(std::variant_size_v<ExprPtrVariant> == 15, "Looks like you forgot to update the cases in Evaluator::Evaluate(const ExptrVariant&)!");
            return "";
        }
    }

    // ----------------- Statement Evaluation Methods -----------------

    auto Evaluator::evaluate_expr_stmt(const ExprStmtPtr &stmt) -> std::optional<csm_object> {
#ifdef EVAL_DEBUG
//   ErrorsAndDebug::debugPrint("evaluate_expr_stmt called.");
#endif // EVAL_DEBUG

        csm_object result = evaluate_expr(stmt->expression);

#ifdef EVAL_DEBUG
//   ErrorsAndDebug::debugPrint("evaluate_expr_stmt: expression evaluation result: " + getObjectString(result));
#endif // EVAL_DEBUG
        return std::nullopt;
    }

    auto Evaluator::evaluate_print_stmt(const PrintStmtPtr &stmt) -> std::optional<csm_object> {
#ifdef EVAL_DEBUG
//   ErrorsAndDebug::debugPrint("evaluatePrintStmt called.");
#endif // EVAL_DEBUG

        csm_object objectToPrint = evaluate_expr(stmt->expression);
        std::cout << ">" << get_object_string(objectToPrint) << std::endl;

#ifdef EVAL_DEBUG
        ErrorsAndDebug::debugPrint("evaluatePrintStmt should have printed." + getObjectString(objectToPrint));
#endif // EVAL_DEBUG
        return std::nullopt;
    }

    auto Evaluator::evaluate_block_stmt(const BlockStmtPtr &stmt) -> std::optional<csm_object> {
        auto currEnviron = environManager.get_curr_env();
        environManager.create_new_environ();
        std::optional<csm_object> result = evaluate_stmts(stmt->statements);
        environManager.discard_environs_till(currEnviron);
        return result;
    }

    auto Evaluator::evaluate_var_stmt(const VarStmtPtr &stmt) -> std::optional<csm_object> {
        if (stmt->initializer.has_value()) {
            environManager.define(stmt->varName, evaluate_expr(stmt->initializer.value()));
        }
        else {
            environManager.define(stmt->varName, csm_object(nullptr));
        }
        return std::nullopt;
    }

    auto Evaluator::evaluate_if_stmt(const IfStmtPtr &stmt) -> std::optional<csm_object> {
        if (is_true(evaluate_expr(stmt->condition)))
            return evaluate_stmt(stmt->thenBranch);
        if (stmt->elseBranch.has_value())
            return evaluate_stmt(stmt->elseBranch.value());
        return std::nullopt;
    }

    auto Evaluator::evaluate_while_stmt(const WhileStmtPtr &stmt) -> std::optional<csm_object> {
        std::optional<csm_object> result = std::nullopt;
        while (is_true(evaluate_expr(stmt->condition)) && !result.has_value()) {
            result = evaluate_stmt(stmt->loopBody);
        }
        return result;
    }

    auto Evaluator::evalaute_for_stmt(const ForStmtPtr &stmt) -> std::optional<csm_object> {
        std::optional<csm_object> result = std::nullopt;
        if (stmt->initializer.has_value())
            evaluate_stmt(stmt->initializer.value());
        while (true) {
            if (stmt->condition.has_value() && !is_true(evaluate_expr(stmt->condition.value())))
                break;
            result = evaluate_stmt(stmt->loopBody);
            if (result.has_value())
                break;
            if (stmt->increment.has_value())
                evaluate_expr(stmt->increment.value());
        }
        return result;
    }

    auto Evaluator::evalaute_func_stmt(const FuncStmtPtr &stmt) -> std::optional<csm_object> {
        std::shared_ptr<Environment> closure = environManager.get_curr_env();

        environManager.define(stmt->funcName, std::make_shared<FuncObj>(stmt->funcExpr, stmt->funcName.get_lexeme(), std::move(closure)));
        environManager.create_new_environ();
        return std::nullopt;
    }

    auto Evaluator::evaluate_ret_stmt(const RetStmtPtr &stmt) -> std::optional<csm_object> {
        return stmt->value.has_value() ? std::make_optional(evaluate_expr(stmt->value.value())) : std::nullopt;
    }

    auto Evaluator::evaluate_class_stmt(const ClassStmtPtr &stmt) -> std::optional<csm_object> {
        auto superClass = [&]() -> std::optional<csm_class_shrd_ptr> {
            if (stmt->superClass.has_value()) {
                auto superClassObj = evaluate_expr(stmt->superClass.value());
                if (!std::holds_alternative<csm_class_shrd_ptr>(superClassObj))
                    // throw ErrorsAndDebug::reportRuntimeError( eReporter, stmt->className, "Superclass must be a class; Can't inherit from non-class");
                    return std::get<csm_class_shrd_ptr>(superClassObj);
            }
            return std::nullopt;
        }();

        environManager.define(stmt->className, csm_object(nullptr));

        if (superClass.has_value()) {
            environManager.create_new_environ();
            environManager.define("super", superClass.value());
        }

        std::vector<std::pair<std::string, csm_object>> methods;
        std::shared_ptr<Environment> closure = environManager.get_curr_env();
        for (const auto &stmt : stmt->methods) {
            const auto &functionStmt = std::get<FuncStmtPtr>(stmt);
            bool isInitializer = functionStmt->funcName.get_lexeme() == "init";
            csm_object method = std::make_shared<FuncObj>(
                functionStmt->funcExpr, functionStmt->funcName.get_lexeme(),
                closure, true, isInitializer);
            methods.emplace_back(functionStmt->funcName.get_lexeme(), method);
        }

        if (superClass.has_value()) {
            environManager.set_curr_env(environManager.get_curr_env()->get_parent_env());
        }

        environManager.assign(stmt->className, std::make_shared<CsmClass>(stmt->className.get_lexeme(), superClass, methods));

        environManager.create_new_environ();

        return std::nullopt;
    }

    auto Evaluator::evaluate_stmt(const AST::StmtPtrVariant &stmt) -> std::optional<csm_object> {
        switch (stmt.index()) {
        case 0: // ExprStmtPtr
            return evaluate_expr_stmt(std::get<0>(stmt));
        case 1: // PrintStmtPtr
            return evaluate_print_stmt(std::get<1>(stmt));
        case 2: // BlockStmtPtr
            return evaluate_block_stmt(std::get<2>(stmt));
        case 3: // VarStmtPtr
            return evaluate_var_stmt(std::get<3>(stmt));
        case 4: // IfStmtPtr
            return evaluate_if_stmt(std::get<4>(stmt));
        case 5: // WhileStmtPtr
            return evaluate_while_stmt(std::get<5>(stmt));
        case 6: // ForStmtPtr
            return evalaute_for_stmt(std::get<6>(stmt));
        case 7: // FuncStmtPtr
            return evalaute_func_stmt(std::get<7>(stmt));
        case 8: // RetStmtPtr
            return evaluate_ret_stmt(std::get<8>(stmt));
        case 9: // ClassStmtPtr
            return evaluate_class_stmt(std::get<9>(stmt));
        default:
            static_assert(
                std::variant_size_v<StmtPtrVariant> == 10, "Looks like you forgot to update the cases in PrettyPrinter::toString(const StmtPtrVariant& statement)!");
            return std::nullopt;
        }
    }

    auto Evaluator::evaluate_stmts(const std::vector<AST::StmtPtrVariant> &stmts) -> std::optional<csm_object> {
        std::optional<csm_object> result = std::nullopt;
        for (const AST::StmtPtrVariant &stmt : stmts) {
            try {
                result = evaluate_stmt(stmt);
                if (result.has_value())
                    break;

                // WIP: } catch (const ErrorsAndDebug::RuntimeError& e) {
            }
            catch (const std::exception &e) {
                //   ErrorsAndDebug::debugPrint("Caught unhandled exception.");
                if (EXPECT_FALSE(++num_runtime_err > MAX_RUNTIME_ERR)) {
                    std::cerr << "Too many errors occurred. Exiting evaluation." << std::endl;
                    throw e;
                }
            }
        }
        return result;
    }

    class clockBuiltin : public BuiltInFunc {
    public:
        explicit clockBuiltin(Environment::environment_ptr closure) : BuiltInFunc("clock", std::move(closure)) {}
        auto arity() -> size_t override { return 0; }
        auto run() -> csm_object override {
            return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
        }
        auto get_fn_name() -> std::string override { return "< builtin-fn_clock >"; }
    };

    // WIP: add ereporter
    Evaluator::Evaluator() {
        environManager.define(Types::Token(TokenType::MISSION, "clock"), static_cast<built_in_func_shrd_ptr>(std::make_shared<clockBuiltin>(environManager.get_curr_env())));
    }

} // namespace cosmos::Evaluator
