
#include "objects.h"

#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

namespace cosmos::Evaluator {

    FuncObj::FuncObj(const AST::FuncExprPtr& declaration, std::string func_name, std::shared_ptr<Environment> closure, bool is_method, bool is_initalizer)
    : declaration(declaration),
    func_name(std::move(func_name)),
    closure(std::move(closure)),
    is_method(is_method),
    is_initalizer(is_initalizer) {};

    auto FuncObj::arity() const -> size_t { return declaration->parameters.size(); }

    auto FuncObj::get_params() const -> const std::vector<Types::Token>& { return declaration->parameters; }

    auto FuncObj::get_closure() const -> std::shared_ptr<Environment> { return closure; }

    auto FuncObj::get_decl() const -> const AST::FuncExprPtr& { return declaration; }

    auto FuncObj::get_fn_body_stmts() const -> const std::vector<AST::StmtPtrVariant>& { return declaration->body ;}

    auto FuncObj::get_fn_name() const -> const std::string& { return func_name; }

    auto FuncObj::get_is_method() const -> bool { return is_method; }

    auto FuncObj::get_is_initalizer() const -> bool { return is_initalizer; }



    BuiltInFunc::BuiltInFunc(std::string func_name, std::shared_ptr<Environment> closure) : func_name(std::move(func_name)), closure(std::move(closure)) {}



    CsmClass::CsmClass(std::string class_name, std::optional<csm_class_shrd_ptr> super_class, const std::vector<std::pair<std::string, csm_object>>& method_pairs)
    : class_name(class_name), 
    super_class(std::move(super_class)) {
        for(const std::pair<std::string, csm_object>& m_pair : method_pairs){
            methods.insert_or_assign(hasher(m_pair.first), m_pair.second);
        }
    }
    auto CsmClass::get_class_name() -> std::string { return class_name; }
    
    auto CsmClass::get_super_class() -> std::optional<csm_class_shrd_ptr> {
        return super_class; 
    }

    auto CsmClass::find_method(const std::string& method_name) -> std::optional<csm_object> {
        auto iter = methods.find(hasher(method_name));
        if(iter != methods.end()) return iter->second;

        if(super_class.has_value()){
            return super_class.value()->find_method(method_name);
        }

        return std::nullopt;
    }




    CsmInstance::CsmInstance(csm_class_shrd_ptr klass) : klass(std::move(klass)){}

    auto CsmInstance::to_string() -> std::string {
        return "Instance of " + klass->get_class_name();
    }

    auto CsmInstance::get(const std::string& prop_name) -> csm_object {
        auto iter = fields.find(hasher(prop_name));
        if(iter != fields.end()){
            return iter->second;
        }
        std::optional<csm_object> method = klass->find_method(prop_name);
        if(method.has_value()) return method.value();

        // WIP: Runtime error 
    }


    void CsmInstance::set(const std::string& prop_name, csm_object value){
        fields[hasher(prop_name)] = std::move(value);
    }

    // csm_object Functions
    auto are_equal(const csm_object& left, const csm_object& right) -> bool {
      if (left.index() == right.index()) {
        switch (left.index()) {
          case 0:  // string
            return std::get<std::string>(left) == std::get<std::string>(right);
          case 1:  // double
            return std::get<double>(left) == std::get<double>(right);
          case 2:  // bool
            return std::get<bool>(left) == std::get<bool>(right);
          case 3:  // std::nullptr_t
            // The case where one is null and the other isn't is handled by the
            // outer condition;
            return true;
          case 4:  // FuncShrdPtr
            return std::get<func_shrd_ptr>(left)->get_fn_name()
                   == std::get<func_shrd_ptr>(right)->get_fn_name();
          case 5:  // BuiltinFuncShrdPtr
            return std::get<built_in_func_shrd_ptr>(left)->get_fn_name()
                   == std::get<built_in_func_shrd_ptr>(right)->get_fn_name();
          case 6:  // LoxClassShrdPtr
            return std::get<csm_class_shrd_ptr>(left)->get_class_name()
                   == std::get<csm_class_shrd_ptr>(right)->get_class_name();
          case 7:  // LoxInstanceShrdPtr
            return std::get<csm_instance_shrd_ptr>(left).get()
                   == std::get<csm_instance_shrd_ptr>(right).get();
          default:
            static_assert(std::variant_size_v<csm_object> == 8,
                          "Looks like you forgot to update the cases in "
                          "ExprEvaluator::areEqual(const csm_object&, const "
                          "csm_object&)!");
        }
      }
      return false;
    }

    auto get_object_string(const csm_object& object) -> std::string {
      switch (object.index()) {
        case 0:  // string
          return std::get<0>(object);
        case 1: {  // double
          std::string result = std::to_string(std::get<1>(object));
          auto pos = result.find(".000000");
          if (pos != std::string::npos)
            result.erase(pos, std::string::npos);
          else
            result.erase(result.find_last_not_of('0') + 1, std::string::npos);
          return result;
        }
        case 2:  // bool
          return std::get<2>(object) == true ? "true" : "false";
        case 3:  // nullptr
          return "nil";
        case 4:  // FuncShrdPtr
          return std::get<func_shrd_ptr>(object)->get_fn_name();
        case 5:  // BuiltinFuncShrdPtr
          return std::get<built_in_func_shrd_ptr>(object)->get_fn_name();
        case 6:  // LoxClassShrdPtr
          return std::get<csm_class_shrd_ptr>(object)->get_class_name();
        case 7:  // LoxInstanceShrdPtr
          return std::get<csm_instance_shrd_ptr>(object)->to_string();
        default:
          static_assert(std::variant_size_v<csm_object> == 8,
                        "Looks like you forgot to update the cases in "
                        "getLiteralString()!");
          return "";
      }
    }

    auto is_true(const csm_object& object) -> bool {
      if (std::holds_alternative<std::nullptr_t>(object)) return false;
      if (std::holds_alternative<bool>(object)) return std::get<bool>(object);
      if (std::holds_alternative<func_shrd_ptr>(object))
        return (std::get<func_shrd_ptr>(object) == nullptr);
      if (std::holds_alternative<built_in_func_shrd_ptr>(object))
        return (std::get<built_in_func_shrd_ptr>(object) == nullptr);
      if (std::holds_alternative<csm_class_shrd_ptr>(object))
        return (std::get<csm_class_shrd_ptr>(object) == nullptr);
      if (std::holds_alternative<csm_instance_shrd_ptr>(object))
        return (std::get<csm_instance_shrd_ptr>(object) == nullptr);
      return true;  // for all else we go to true
    }

} // namespace cosmos::Evaluator






