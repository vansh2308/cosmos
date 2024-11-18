
#ifndef COSMOS_AST_PRETTYPRINTER__H
#define COSMOS_AST_PRETTYPRINTER__H

#include <string>
#include <vector>

#include "AST/nodeTypes.h"

namespace cosmos::AST::PrettyPrinter {

    [[nodiscard]] auto to_string(const std::vector<AST::StmtPtrVariant>& statements) -> std::vector<std::string>;
    [[nodiscard]] auto to_string(const ExprPtrVariant& expression) -> std::string;
    [[nodiscard]] auto to_string(const StmtPtrVariant& statement) -> std::vector<std::string>;

}  // namespace cosmos::AST::PrettyPrinter

#endif 