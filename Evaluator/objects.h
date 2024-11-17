#ifndef COSMOS_EVALUATOR_FUNCTION__H
#define COSMOS_EVALUATOR_FUNCTION__H
#pragma once

#include <map>
#include <memory>
#include <string>
#include <variant>

#include "Types/uncopyable.h"
#include "Types/token.h"
#include "AST/nodeTypes.h"

namespace cosmos::Evaluator{
    class FuncObj;
    using func_shrd_ptr = std::shared_ptr<FuncObj>;

    class BuiltInFunc;
    using built_in_func_shrd_ptr = std::shared_ptr<BuiltInFunc>;

    class CsmClass;
    using csm_class_shrd_ptr = std::shared_ptr<CsmClass>;

    class CsmInstance;
    using csm_instance_shrd_ptr = std::shared_ptr<CsmInstance>;
    using csm_object = std::variant<std::string, double, bool, std::nullptr_t, func_shrd_ptr, built_in_func_shrd_ptr, csm_class_shrd_ptr, csm_instance_shrd_ptr>;

    auto are_equal(const csm_object& left, const csm_object& right) -> bool;
    auto get_object_string(const csm_object& obj) -> std::string;
    auto is_true(const csm_object& obj) -> bool;

    class Environment;

    class FuncObj : public Types::Uncopyable {
        const AST::FuncExprPtr& declaration;
        const std::string func_name;
        std::shared_ptr<Environment> closure;
        bool is_method;
        bool is_initalizer;

        public:
        explicit FuncObj(const AST::FuncExprPtr& declaration, std::string func_name, std::shared_ptr<Environment> closure, bool is_method = false, bool is_initalizer = false);

        [[nodiscard]] auto arity() const -> size_t;
        [[nodiscard]] auto get_closure() const -> std::shared_ptr<Environment>;
        [[nodiscard]] auto get_decl() const -> const AST::FuncExprPtr&;
        [[nodiscard]] auto get_fn_body_stmts() const -> const std::vector<AST::StmtPtrVariant>&;
        [[nodiscard]] auto get_fn_name() const -> const std::string&;
        [[nodiscard]] auto get_is_method() const -> bool;
        [[nodiscard]] auto get_is_initalizer() const -> bool;
        [[nodiscard]] auto get_params() const -> const std::vector<Types::Token>&;
    };


    class BuiltInFunc : public Types::Uncopyable {
        std::string func_name = "";
        std::shared_ptr<Environment> closure;

        public:
        explicit BuiltInFunc(std::string func_name, std::shared_ptr<Environment> closure);

        virtual auto arity() -> size_t = 0;
        virtual auto run() -> csm_object = 0;
        virtual auto get_fn_name() -> std::string = 0;
    };



    class CsmClass : public Types::Uncopyable {
        const std::string class_name;
        std::optional<csm_class_shrd_ptr> super_class;
        std::hash<std::string> hasher;
        std::map<size_t, csm_object> methods;

        public:
        explicit CsmClass(std::string class_name, std::optional<csm_class_shrd_ptr> super_class, const std::vector<std::pair<std::string, csm_object>>& method_pairs);
        auto get_class_name() -> std::string;
        auto get_super_class() -> std::optional<csm_class_shrd_ptr>;
        auto find_method(const std::string& method_name) -> std::optional<csm_object>;
    };


    class CsmInstance : public Types::Uncopyable {
        const csm_class_shrd_ptr klass;
        std::hash<std::string> hasher;
        std::map<size_t, csm_object> fields;

        public:
        explicit CsmInstance(csm_class_shrd_ptr klass);
        auto to_string() -> std::string;
        auto get(const std::string& prop_name) -> csm_object;
        void set(const std::string& prop_name, csm_object value);
    };

} // namespace cosmos::Evaluator

#endif
