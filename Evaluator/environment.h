#ifndef COSMOS_EVALUATOR_ENVIRONMENT_H
#define COSMOS_EVALUATOR_ENVIRONMENT_H
#pragma once


#include <cstddef>
#include <exception>
#include <list>
#include <map>
#include <memory>
#include <optional>

#include "objects.h"
#include "../Types/token.h"
#include "../Types/uncopyable.h"

namespace cosmos::Evaluator{
    // WIP: Add debugger 

    class Environment : public Types::Uncopyable, std::enable_shared_from_this<Environment> {
        public:
        using environment_ptr = std::shared_ptr<Environment>;

        explicit Environment(environment_ptr parent_environ);
        auto assign(size_t hashed_var_name, csm_object obj) -> bool;
        void define(size_t hashed_var_name, csm_object obj);
        auto get(size_t hashed_var_name) -> csm_object;
        auto get_parent_env() -> environment_ptr;
        auto is_global() -> bool;


        private:
        std::map<size_t, csm_object> objects;
        environment_ptr parent_environ = nullptr;
    };


    class EnvironmentManager : public Types::Uncopyable {
        public:
        // WIP: explicit EnvironmentManager((ErrorReporter& eReporter));
        explicit EnvironmentManager();

        void assign(const Types::Token& var_token, csm_object obj);
        void create_new_environ(const std::string& caller = __builtin_FUNCTION());
        void discard_environs_till(const Environment::environment_ptr& env_to_restore, const std::string& caller = __builtin_FUNCTION());
        void define(const std::string& token_str, csm_object obj);
        void define(const Types::Token& var_token, csm_object obj);
        auto get(const Types::Token& var_token) -> csm_object;
        auto get(const std::string& var_token) -> csm_object;
        auto get_curr_env() -> Environment::environment_ptr;
        void set_curr_env(Environment::environment_ptr new_curr, const std::string& caller = __builtin_FUNCTION()) ;

        private:
        // WIP: add error reporter 
        Environment::environment_ptr curr_env;
        std::hash<std::string> hasher;
    };


} // namespace cosmos::Evaluator


#endif