
#include "environment.h"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <variant>

#define EXPECT_TRUE(x) __builtin_expect(static_cast<int64_t>(x), 1)
#define EXPECT_FALSE(x) __builtin_expect(static_cast<int64_t>(x), 0)

namespace cosmos::Evaluator{
    namespace {
        class UndefinedVarAccess : public std::exception {};
        class UninitializedVarAccess : public std::exception {};
    }


    // ––––––––––––––––– CLass Environment –––––––––––––––––

    Environment::Environment(environment_ptr parent_env) :
    parent_environ(std::move(parent_env)) {}

    auto Environment::assign(size_t hashed_var_name, csm_object obj) -> bool {
        auto iter = objects.find(hashed_var_name);
        if(iter != objects.end()){
            objects.insert_or_assign(hashed_var_name, obj);
            return true;
        }

        if(EXPECT_TRUE(parent_environ != nullptr)) return parent_environ->assign(hashed_var_name, std::move(obj));
        throw UndefinedVarAccess();
    }


    void Environment::define(size_t hashed_var_name, csm_object obj){
        objects.insert_or_assign(hashed_var_name, obj);
    }

    auto Environment::get(size_t hashed_var_name) -> csm_object {
        auto iter = objects.find(hashed_var_name);
        if(EXPECT_TRUE(iter != objects.end())){
            if(EXPECT_FALSE(std::holds_alternative<std::nullptr_t>(iter->second))) {
                throw UninitializedVarAccess();
            }
            return iter->second;
        }

        if(EXPECT_TRUE(parent_environ != nullptr)) return parent_environ->get(hashed_var_name);

        throw UndefinedVarAccess();
    }

    auto Environment::is_global() -> bool { return (parent_environ == nullptr); }

    auto Environment::get_parent_env() -> environment_ptr { return parent_environ; }



    // ––––––––––––––––– CLass EnvironmentManager –––––––––––––––––
    EnvironmentManager::EnvironmentManager()
    :curr_env(std::make_shared<Environment>(nullptr)) {
        #ifdef ENVIRON_DEBUG
        // WIP: ErrorDebugger 
        #endif
    }

    void EnvironmentManager::create_new_environ(const std::string& caller){
        curr_env = std::make_shared<Environment>(curr_env);
        #ifdef ENVIRON_DEBUG
        // WIP: Error dbugger 
        #endif
    }

    void EnvironmentManager::discard_environs_till(const Environment::environment_ptr& env_to_restore, const std::string& caller){
        #ifdef ENVIRON_DEBUG
        // WIP: Error dbugger 
        #endif

        while(EXPECT_TRUE(!curr_env->is_global() && curr_env.get() != env_to_restore.get())) {
            #ifdef ENVIRON_DEBUG
            // WIP: Error dbugger 
            #endif

            curr_env = curr_env->get_parent_env();
        }
    }



    void EnvironmentManager::define(const std::string& token_str, csm_object obj){
        curr_env->define(hasher(token_str), std::move(obj));
    }

    void EnvironmentManager::define(const Types::Token& var_token, csm_object obj){
        curr_env->define(hasher(var_token.get_lexeme()), std::move(obj));
    }

    void EnvironmentManager::assign(const Types::Token& var_token, csm_object obj){
        try{
            curr_env->assign(hasher(var_token.get_lexeme()), std::move(obj));
        } catch (const UndefinedVarAccess& e){
            // WIP: Error dbugger 
        }
    }


    auto EnvironmentManager::get(const Types::Token& var_token) -> csm_object{
        try{
            return curr_env->get(hasher(var_token.get_lexeme()));
        } catch (const UndefinedVarAccess& e){
            // WIP: Error dbugger 
        } catch (const UninitializedVarAccess& e){
            // WIP: Error dbugger 
        }
    }



    auto EnvironmentManager::get_curr_env() -> Environment::environment_ptr{
        return curr_env;
    }

    void EnvironmentManager::set_curr_env(Environment::environment_ptr new_curr, const std::string& caller) {
        #ifdef ENVIRON_DEBUG
        // WIP: Error dbugger 
        #endif
        
        curr_env = std::move(new_curr);
    }
    
} // namespace cosmos::Evaluator 