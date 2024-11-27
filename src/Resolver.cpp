#include "../include/Resolver.hpp"
#include "../include/Logger.hpp"

Resolver::Resolver(Interpreter& interpreter) : interpreter{interpreter} {
    func_stack.push(FuncType::NONE);
}

void Resolver::resolve(const std::vector<unique_stmt_ptr>& statements) {
    for (const auto& stmt : statements) {
        assert(stmt);
        resolve(*stmt);
    }
}

void Resolver::resolve(const Stmt& stmt) {
    stmt.accept(*this);
}

void Resolver::resolve(const Expr& expr) {
    expr.accept(*this);
}

void Resolver::resolveLocal(const Expr* expr, const Token& identifier) {
    if (scopes.empty())
        return;
    for (auto scope = scopes.rbegin(); scope != scopes.rend(); ++scope) {
        if (scope->contains(identifier.lexeme)) {
            interpreter.resolve(*expr, std::distance(scopes.rbegin(), scope));
            return;
        }
    }
}

void Resolver::resolveFunction(const FnStmt& stmt, FuncType type) {

    func_stack.push(type);
    beginScope();

    for (const auto& param : stmt.params) {
        declare(param);
        define(param);
    }

    resolve(stmt.body);
    endScope();
    func_stack.pop();
}

void Resolver::beginScope() {
    scopes.emplace_back();
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(const Token& identifier) {
    if (scopes.empty())
        return;

    Scope& scope = scopes.back();
    if (scope.contains(identifier.lexeme)) {
        Error::addError(identifier, "Variable with the name '" + identifier.lexeme + "' already exists in this scope");
    }
    scope.try_emplace(identifier.lexeme, false);
}

void Resolver::define(const Token& identifier) {
    if (scopes.empty())
        return;
    scopes.back()[identifier.lexeme] = true;
}

std::any Resolver::visit(const BinaryExpr& expr) {
    resolve(*expr.left);
    resolve(*expr.right);
    return {};
}

std::any Resolver::visit(const UnaryExpr& expr) {
    resolve(*expr.right);
    return {};
}

std::any Resolver::visit(const GroupingExpr& expr) {
    resolve(*expr.expression);
    return {};
}

std::any Resolver::visit(const LiteralExpr& expr) {
    return {};
}

std::any Resolver::visit(const AssignExpr& expr) {
    resolve(*expr.value);
    resolveLocal(&expr, expr.identifier);
    return {};
}

std::any Resolver::visit(const CallExpr& expr) {
    resolve(*expr.callee);
    for (const auto& argument : expr.args) {
        resolve(*argument);
    }
    return {};
}

std::any Resolver::visit(const LogicalExpr& expr) {
    resolve(*expr.left);
    resolve(*expr.right);
    return {};
}

std::any Resolver::visit(const SetExpr& expr) {
    return {};
}

std::any Resolver::visit(const GetExpr& expr) {
    return {};
}

std::any Resolver::visit(const SuperExpr& expr) {
    return {};
}

std::any Resolver::visit(const ThisExpr& expr) {
    return {};
}

std::any Resolver::visit(const VarExpr& expr) {
    if (!scopes.empty()) {
        const Scope& scope = scopes.back();
        if (scope.contains(expr.identifier.lexeme) && !scope.at(expr.identifier.lexeme)) {
            Error::addError(expr.identifier, "Can't read local variable in its own initializer.");
        }
    }

    resolveLocal(&expr, expr.identifier);
    return {};
}

std::any Resolver::visit(const ListExpr& expr) {
    for (const auto& item : expr.items) {
        resolve(*item);
    }

    return {};
}

std::any Resolver::visit(const SubscriptExpr& expr) {
    resolve(*expr.index);

    if (expr.value) {
        resolve(*expr.value);
    }
    resolveLocal(&expr, expr.identifier);
    return {};
}

std::any Resolver::visit(const IncrementExpr& expr) {
    resolveLocal(&expr, expr.identifier);
    return {};
}

std::any Resolver::visit(const DecrementExpr& expr) {
    resolveLocal(&expr, expr.identifier);
    return {};
}

void Resolver::visit(const BlockStmt& stmt) {
    beginScope();
    resolve(stmt.statements);
    endScope();
}

void Resolver::visit(const ClassStmt& stmt) {
    // WIP 
}

void Resolver::visit(const ExprStmt& stmt) {
    resolve(*stmt.expression);
}

void Resolver::visit(const FnStmt& stmt) {
    declare(stmt.identifier);
    define(stmt.identifier);
    resolveFunction(stmt, FuncType::FUNCTION);
}

void Resolver::visit(const IfStmt& stmt) {
    resolve(*stmt.main_branch.condition);
    resolve(*stmt.main_branch.statement);

    for (const auto& elif : stmt.elif_branches) {
        resolve(*elif.condition);
        resolve(*elif.statement);
    }

    if (stmt.else_branch) {
        resolve(*stmt.else_branch);
    }
}

void Resolver::visit(const PrintStmt& stmt) {
    resolve(*stmt.expression);
}

void Resolver::visit(const ReturnStmt& stmt) {
    if (func_stack.top() == FuncType::NONE) {
        Error::addError(stmt.keyword, "Can't return from a top-level code.");
    }
    if (stmt.expression) {
        resolve(*stmt.expression);
    }
}

void Resolver::visit(const BreakStmt& stmt) {
    if (loop_nesting_level == 0) {
        Error::addError(stmt.keyword, "Can't break outside of a loop.");
    }
}

void Resolver::visit(const ContinueStmt& stmt) {
    if (loop_nesting_level == 0) {
        Error::addError(stmt.keyword, "Can't continue outside of a loop.");
    }
}

void Resolver::visit(const VarStmt& stmt) {
    declare(stmt.identifier);
    if (stmt.initializer) {
        resolve(*stmt.initializer);
    }

    define(stmt.identifier);
}

void Resolver::visit(const WhileStmt& stmt) {
    ++loop_nesting_level;
    resolve(*stmt.condition);
    resolve(*stmt.body);
    --loop_nesting_level;
}

void Resolver::visit(const ForStmt& stmt) {
    ++loop_nesting_level;
    beginScope();

    if (stmt.initializer)
        resolve(*stmt.initializer);
    if (stmt.condition)
        resolve(*stmt.condition);
    if (stmt.increment)
        resolve(*stmt.increment);

    resolve(*stmt.body);
    endScope();
    --loop_nesting_level;
}